<div style="text-align: right; padding: 16px; margin-left: 64px; margin-bottom: 16px; background-color: #222222; font-style: italic">
As a DIY project i want to create a 3 axis handheld camera gimbal. I have a working single axis POC in place where a Bosch BNO055 IMU reads orientation every 10ms and the z rading is applied as target position to the motor. The motor is driven with the SimpleFOC library, which is working fine.
Yet, it appears there is lag between reading from the IMU and the time the sensor value can actually be applied to the motor. Therefore I am searching for a good pattern to predict the immediate future movements in a 10 to 50 ms range. Can you please point me to a pattern or algorithm on how to achieve this prediction.
</div>

The problem you're describing — compensate for end-to-end latency by predicting orientation a few tens of ms ahead — is exactly the "motion-to-photon latency compensation" problem from VR/AR head tracking. Oculus, HoloLens, etc. all use IMU-based pose extrapolation for the same reason (their prediction horizon is usually 20–50 ms too). That's the field to borrow from if you want to go deeper, but the core techniques are straightforward and you don't need anything exotic for a gimbal.

1. Use angular velocity to extrapolate — not just the fused angle

The BNO055 gives you gyroscope output alongside the fused orientation. Rather than trying to predict from the angle signal alone, use the measured angular rate ω to do a first-order (constant-velocity) Taylor expansion:

```
θ_predicted(t + Δt) = θ_now + ω_z \* Δt
```

Δt here is your measured/estimated total latency (IMU read + fusion delay + control loop + motor response), not your sample period. Measure that end-to-end lag empirically (step response test: rotate sharply, compare motor position timestamp to IMU event timestamp) — it's usually more than people expect once you include I2C read time and BNO055's internal fusion delay.

If you want a bit more accuracy for fast hand-jerk motions, add a second-order term using angular acceleration (α = dω/dt, estimated by differencing consecutive gyro readings):

```
θ_predicted(t + Δt) = θ_now + ω_z*Δt + 0.5*α_z\*Δt²
```

Be careful here — differencing raw gyro to get α is noisy, and squaring Δt amplifies that noise. Only add the second-order term if you low-pass filter α first, and keep Δt small (your 10–50 ms range is fine, but don't push it further).

2. Get a cleaner velocity/acceleration estimate before extrapolating

Raw gyro is noisy enough that naive extrapolation can introduce jitter or overshoot on fast motions. Two standard patterns for getting a smoother velocity/acceleration estimate to feed into the extrapolation:

Alpha-beta(-gamma) filter: a fixed-gain tracker (essentially a simplified steady-state Kalman filter) that maintains smoothed estimates of position, velocity, and optionally acceleration, updated each sample. Cheap enough for any MCU, no matrix math, easy to tune with two or three gain constants.
Kalman filter with a constant-velocity or constant-acceleration state model: state = [θ, ω, (α)], predict step propagates state forward by Δt, update step corrects with the new IMU reading. This is the "textbook" version of what VR trackers do, and gives you the prediction step almost for free since Kalman filters are built around predict/update cycles.

Either way, the pattern is: filter → get a clean rate estimate → extrapolate that rate forward by your known latency. Don't extrapolate raw/noisy signals directly.

3. Consider whether you need explicit prediction at all — rate feedforward

Worth knowing: production gimbal controllers (SimpleBGC/BaseCam, Storm32, ArduPilot's gimbal stabilization) mostly don't do explicit trajectory prediction. Instead they use a cascaded controller: a position loop plus a feedforward term derived directly from gyro rate, added to the PID output rather than to the setpoint. This compensates for lag without amplifying noise the way position extrapolation can, and it's usually more robust in practice. If your current single-axis loop is pure position control (angle in → angle target → SimpleFOC), adding a rate feedforward term is often a bigger win than prediction, and it's simpler to implement. Many people end up combining both: a modest predictive extrapolation (order of your measured latency) plus rate feedforward in the control loop.

4. Also check where the lag is actually coming from

Before building a predictor, it's worth confirming the lag isn't dominated by something fixable:

BNO055's internal sensor fusion has its own latency (varies by fusion mode — NDOF fusion is smoother but slower than IMU-only mode).
Reading fused quaternion/Euler over I2C at 10 ms intervals caps you at 100 Hz, which is low for a handheld gimbal — most gimbal control loops run at 1 kHz+ for the motor side. If your SimpleFOC loop runs faster than 100 Hz but only gets a new target every 10 ms, you also want to extrapolate between IMU samples, not just apply latency compensation at sample time — same math as above, just also running at your control loop rate.
Using raw gyro (not the fused orientation output) for the fast/high-rate part of your prediction avoids inheriting BNO055's internal fusion delay.

Suggested starting point: alpha-beta filter (or simple low-pass on gyro) feeding a first-order extrapolation with Δt = your measured latency, plus a rate feedforward term in the SimpleFOC control loop. That covers the 10–50 ms range well without the complexity of a full EKF, and matches what's actually used in commercial gimbal firmware.

<div style="text-align: right; padding: 16px; margin-left: 64px; margin-bottom: 16px; background-color: #222222; font-style: italic">
Thank you. Can you please give more detail on how to set a rate feedforward term in SimpleFOC.
</div>

Here's how to structure this in SimpleFOC. The key issue is that SimpleFOC's built-in MotionControlType::angle mode runs a fixed cascade (angle error → P controller → velocity target → velocity PID → voltage), and there's no clean hook to inject a feedforward term into that internal cascade. So the standard approach is: take over the angle→velocity step yourself, and add the feedforward there, then hand off to SimpleFOC's velocity loop (which still does the torque/voltage work and handles FOC commutation).

1. Configure the motor for velocity control, not angle control

<sub>cpp</sub>

```
motor.controller = MotionControlType::velocity;
// motor.PID_velocity.P, .I, .D, .output_ramp, .limit — tune as usual
```

You keep SimpleFOC's PID_velocity and loopFOC() exactly as-is. You're only replacing the outer position loop.

2. Implement your own angle→velocity step, with feedforward added

<sub>cpp</sub>

```
// Your own outer-loop controller (P or PI is usually enough here since
// the feedforward is carrying most of the load)
PIDController P_angle_custom{2.0f, 0.0f, 0.0f, 10000.0f, 1000.0f};
// (P, I, D, output_ramp, output_limit — tune P first, add I only if you see steady-state offset)

float Kff = 1.0f; // feedforward gain, start here and tune

void loop() {
motor.loopFOC(); // keep this at max frequency, e.g. every cycle

// target_angle, target_rate come from your IMU pipeline:
// target_angle = predicted orientation (angle extrapolation)
// target_rate = filtered/predicted angular velocity (rad/s, gyro-derived)

float angle_error = target_angle - motor.shaft_angle;
float velocity_command = P_angle_custom(angle_error) + Kff \* target_rate;

// optional: clamp to your velocity limit if PIDController's own limit isn't enough
velocity_command = constrain(velocity_command, -motor.velocity_limit, motor.velocity_limit);

motor.move(velocity_command);
}
```

Why this works: the feedforward term (Kff \* target_rate) tells the motor "you should already be moving at this speed" before any position error has accumulated. The P controller on angle error then only has to correct residual drift/noise, not drive the whole motion. This is what removes the lag — instead of waiting for an angle error to appear and then reacting to it, the loop is pre-emptively commanding the right velocity.

3. Tuning Kff

Units must match: if target_rate is in rad/s and motor.move() in velocity mode expects rad/s, start with Kff = 1.0.
If the gimbal still lags visibly on fast pans → increase Kff slightly (values like 1.0–1.3 are common, since real systems have some viscous friction/inertia that pure kinematic feedforward doesn't account for).
If it overshoots or oscillates on fast motions → reduce Kff, or check that your rate estimate isn't noisy (this ties back to filtering the gyro before using it — an alpha-beta filter or low-pass on the raw gyro rate, as discussed before).
Drop your outer P_angle_custom gain lower than you'd use without feedforward — its job now is just trimming residual error, and too much P on top of feedforward causes it to fight the FF term and oscillate.

4. Combining with the prediction from before

You now have two compensation mechanisms working together:

target_angle = extrapolated angle (θ + ω·Δt) — compensates for known fixed latency
target_rate (feedforward) = current/filtered angular rate — compensates for the lag inherent in cascaded control, independent of prediction horizon

These are complementary, not redundant — the angle extrapolation shifts your setpoint forward in time, and the rate feedforward makes the motor track that setpoint without the usual PID reaction delay. It's fine to run both, but tune them one at a time: get the feedforward-only loop stable and responsive first, then layer in angle extrapolation and see whether it still helps or starts to overcompensate. If it overcompensates, reduce your Δt in the extrapolation before touching Kff.

5. Optional next step — inertia feedforward

If you find velocity feedforward alone still lags during sharp accelerations (fast wrist snaps), you can add a torque feedforward term proportional to angular acceleration: torque_ff = J \* α, where J is your gimbal arm's estimated moment of inertia and α is your filtered angular acceleration. This adds directly to the torque/voltage command rather than the velocity command — more advanced, and only worth doing once velocity feedforward is dialed in and you're still seeing lag specifically during fast accel/decel transients.

One practical note: make sure your IMU frame and motor shaft direction conventions agree in sign before tuning gains — an inverted sign on target_rate will look like instability/oscillation and can easily be mistaken for a bad Kff value.

<div style="text-align: right; padding: 16px; margin-left: 64px; margin-bottom: 16px; background-color: #222222; font-style: italic">
Thank you. Can you please provide configurable alpha-beta filter example code for arduino.
</div>

Here's a configurable alpha-beta-gamma filter class, structured to plug directly into the pipeline we've discussed (smoothed angle + velocity feed into your prediction/feedforward).

<sub>cpp</sub>

```
// alpha_beta_filter.h
//
// Configurable alpha-beta(-gamma) filter for tracking angle, angular
// velocity, and (optionally) angular acceleration from a noisy IMU signal.
//
// - alpha: how much you trust the new measurement for POSITION correction
// - beta: how much you trust the new measurement for VELOCITY correction
// - gamma: how much you trust the new measurement for ACCELERATION correction
// (only used if useAcceleration = true)
//
// Higher gains -> faster response, more noise passthrough
// Lower gains -> smoother output, more lag
//
// Typical starting points for a handheld gimbal at 100-1000Hz update rate:
// alpha = 0.6-0.85, beta = 0.3-0.5, gamma = 0.05-0.15 (if used)

#ifndef ALPHA_BETA_FILTER_H
#define ALPHA_BETA_FILTER_H

class AlphaBetaFilter {
public:
AlphaBetaFilter(float alpha, float beta, float gamma = 0.0f, bool useAccel = false)
: \_alpha(alpha), \_beta(beta), \_gamma(gamma), \_useAccel(useAccel),
\_x(0), \_v(0), \_a(0), \_initialized(false) {}

    // Call once with a known starting angle before the control loop begins,
    // otherwise the filter will "swing in" from zero over the first samples.
    void init(float initialAngle) {
        _x = initialAngle;
        _v = 0.0f;
        _a = 0.0f;
        _initialized = true;
    }

    // measurement: raw angle reading (rad or deg, be consistent throughout)
    // dt: time since last update, in seconds
    // returns: filtered angle
    float update(float measurement, float dt) {
        if (!_initialized) {
            init(measurement);
            return _x;
        }
        if (dt <= 0.0f) return _x;  // guard against bad timestamps

        // --- Predict step ---
        float x_pred = _x + _v * dt + (_useAccel ? 0.5f * _a * dt * dt : 0.0f);
        float v_pred = _v + (_useAccel ? _a * dt : 0.0f);
        float a_pred = _a;

        // --- Correct step ---
        float residual = measurement - x_pred;

        _x = x_pred + _alpha * residual;
        _v = v_pred + (_beta / dt) * residual;

        if (_useAccel) {
            _a = a_pred + (2.0f * _gamma / (dt * dt)) * residual;
        }

        return _x;
    }

    // Extrapolate the current filtered state forward by latency dt_future
    // (this is the "predict Δt ahead" step from the earlier discussion)
    float predict(float dt_future) const {
        return _x + _v * dt_future
             + (_useAccel ? 0.5f * _a * dt_future * dt_future : 0.0f);
    }

    float getAngle()        const { return _x; }
    float getVelocity()     const { return _v; }
    float getAcceleration() const { return _a; }

    void setGains(float alpha, float beta, float gamma = 0.0f) {
        _alpha = alpha;
        _beta = beta;
        _gamma = gamma;
    }

    void reset() { _initialized = false; }

private:
float \_alpha, \_beta, \_gamma;
bool \_useAccel;
float \_x, \_v, \_a;
bool \_initialized;
};

#endif
```

Usage in your control loop:

<sub>cpp</sub>

```
#include "alpha_beta_filter.h"

// Start with velocity-only tracking (gamma/useAccel off) — add acceleration
// tracking later only if you find you need the 2nd-order extrapolation term
AlphaBetaFilter yawFilter(0.75f, 0.4f);

unsigned long lastMicros;
const float LATENCY_COMPENSATION_S = 0.03f; // your measured end-to-end lag, e.g. 30ms

void setup() {
// ... IMU + SimpleFOC init ...
lastMicros = micros();
yawFilter.init(readImuZAngle()); // seed with real starting angle
}

void loop() {
motor.loopFOC();

    unsigned long now = micros();
    float dt = (now - lastMicros) * 1e-6f;
    lastMicros = now;

    float rawAngle = readImuZAngle(); // your BNO055 read
    yawFilter.update(rawAngle, dt);

    // Predicted target Δt ahead, using smoothed velocity
    float target_angle = yawFilter.predict(LATENCY_COMPENSATION_S);
    float target_rate   = yawFilter.getVelocity();

    float angle_error = target_angle - motor.shaft_angle;
    float velocity_command = P_angle_custom(angle_error) + Kff * target_rate;

    motor.move(velocity_command);

}
```

Tuning notes:

Tune alpha and beta together, not separately. A common heuristic (from the original alpha-beta-gamma literature) keeps beta ≈ alpha² / (2 - alpha) as a rough starting relationship for a well-damped response — but for your application, empirical tuning against real hand-shake motion will serve you better than the formula. Start with alpha around 0.7, beta around 0.3, and watch the filtered angle on a serial plotter against the raw signal while shaking the rig by hand.
If output\_ f oscillates/rings on sudden moves: lower both gains together, particularly beta.
If it feels laggy/mushy: raise alpha first (position responsiveness), then beta only if velocity feels sluggish to respond.
Leave useAccel = false initially. Get position+velocity tracking solid first — acceleration tracking (gamma) adds noise sensitivity and is only worth enabling if you specifically need the second-order term in predict() for very fast wrist-snap motions, as discussed earlier.
dt must be measured, not assumed. Don't hardcode 10ms even if that's your nominal loop rate — jitter in I2C read time or loop timing will bias the velocity/acceleration estimates if dt is wrong. The micros() based dt in the example handles this.
Run one AlphaBetaFilter instance per axis (yaw/pitch/roll) once you extend past your current single-axis POC — each axis has independent dynamics and may need different gains, especially pitch (often faster hand motion) vs yaw.

One thing worth flagging: this filter operates on a single scalar angle per axis, which is fine for your current single-axis POC. When you move to 3-axis, doing this per-Euler-angle can hit gimbal lock issues near ±90° pitch — at that point you may want to filter the gyro rate vector and orientation quaternion directly rather than three independent Euler angle filters. Not a concern yet for your yaw axis, but worth keeping in mind before you generalize this to pitch/roll.

<div style="text-align: right; padding: 16px; margin-left: 64px; margin-bottom: 16px; background-color: #222222; font-style: italic">
Thank you. I was able to build a working prototype from your suggestions. There is one problem remaining though. The

motor.shaft_angle
value overshoots on fast input, then is slow to return to

target_angle
I have attached a teleplot csv file where "ab0" is the AlphaBeta filter output without prediction, "abi" is the AlphaBeta filter output with 0.03 seconds prediction and "rad" is the motor.shaft_angle. Can you please suggest how to address this issue?

</div>

Good, I pulled the actual data out of the teleplot export. Here's what it shows (timestamps relative to start of your log):

The real motion (ab0, the filtered signal without prediction) rises from about -2.53 to its final value and is essentially flat by t≈0.6s.
rad (motor.shaft_angle) crosses over and starts overshooting abi around t≈0.48s — right as the real motion is finishing its deceleration.
Overshoot peaks at +0.11 to +0.12 rad (≈6.5°) around t≈0.61s, i.e. roughly 130ms after the real motion had already stopped.
It then takes another ~300-400ms to bleed back down, and still isn't fully settled (residual ~0.03 rad) by the end of your log.

That specific shape — motor catches up, sails past the target right as the target decelerates, then takes hundreds of ms to creep back — is the textbook signature of an underdamped, integrator-heavy control loop, not a filtering or prediction problem. Your abi prediction itself tracks reasonably cleanly through the whole move; the divergence is happening downstream of it, in the angle→velocity→motor cascade.

Two likely contributors, in priority order:

1. PID_angle_Y's output limit is effectively disabled, so its integral term can wind up unchecked

cpp
PIDController PID_angle_Y{ 5.000f, 0.250f, 0.0f, 10000.0f, 1000.0f };

SimpleFOC's PIDController clamps the integral state (and output) to ±limit on every call — that's its built-in anti-windup mechanism. But your limit is 1000 rad/s, while motor.velocity_limit is only PI*2*2 ≈ 12.57 rad/s. The clamp never actually engages, so during the sustained tracking error of a fast pan, the integral term can accumulate well past anything the motor can physically use, and then has to slowly unwind once the error reverses sign — matching your "slow to return" symptom almost exactly.

Fix: set the limit to something close to the real usable range, once motor.velocity_limit is known (i.e. in setup(), after motor.init()):

cpp
PID_angle_Y.limit = motor.velocity_limit \* 1.2f; // headroom, but bounded — enables real anti-windup

Also worth adding a D term for damping, and pulling I down somewhat since your feedforward is already doing most of the velocity-tracking work — the outer PID's I term should mainly be mopping up small steady-state bias (friction, gravity offset if any), not driving the fast dynamics:

cpp
PIDController PID_angle_Y{ 5.000f, 0.050f, 0.020f, 10000.0f, 1000.0f }; // I↓, D added

Tune D upward gradually while watching for overshoot reduction — start small (0.01-0.03) since D on a noisy angle-error signal can itself introduce high-frequency jitter if pushed too far.

2. Your inner motor.PID_velocity I/P ratio is high, which is a classic recipe for velocity-loop overshoot

cpp
motor.PID_velocity.P = 0.120;
motor.PID_velocity.I = 0.300;

I is 2.5× P here. When a velocity PID's I term dominates over P like this, the loop tends to overshoot step-like velocity commands and settle slowly — the integral keeps pushing current/torque for a while after the commanded velocity has already dropped, then has to unwind. Given your outer loop delivers something close to a velocity step during a fast pan-then-stop motion, this inner loop is a strong second suspect (and could be compounding with #1 rather than being the sole cause).

Try re-tuning with P dominant and I reduced, e.g. as a starting point:

cpp
motor.PID_velocity.P = 0.200;
motor.PID_velocity.I = 0.120;

Also verify what motor.PID_velocity.limit actually is — you never set it explicitly in the code shown, so it's relying on whatever SimpleFOC defaults to for TorqueControlType::estimated_current. Worth explicitly setting it to motor.current_limit so you know the inner loop's anti-windup clamp is meaningful too, the same way you're fixing it for the outer loop.

How to confirm which loop is the real culprit before you start turning multiple knobs at once

Right now you're only logging position (ab0, abi, rad). Add two more teleplot channels next time you test:

motor.shaft_velocity (actual measured velocity)
velocity_command (what you're sending to motor.move())

If velocity_command itself overshoots and lingers → the problem is upstream, in PID_angle_Y / feedforward (point 1, or filter beta). If velocity_command looks clean but shaft_velocity overshoots and lags behind it → the problem is squarely in motor.PID_velocity (point 2). That single extra log will tell you which fix actually matters most, rather than tuning both blind.

Suggested order of operations

Fix PID_angle_Y.limit first (cheap, clearly wrong regardless of anything else, likely helps immediately).
Add the two extra log channels and re-run the same fast pan-and-stop motion.
Based on where the overshoot shows up, tune either the outer D/I or the inner velocity P/I ratio — not both blindly at once, since they interact and you'll lose track of what fixed what.
Re-check the alpha-beta beta gain (0.10) last, only if overshoot remains — a low beta means the filter's velocity estimate decays slowly on deceleration, which would keep the feedforward term slightly "hot" briefly after real motion stops. It's a much smaller effect than the PID issues above based on this data, but worth a quick check once the loops are cleaned up.
