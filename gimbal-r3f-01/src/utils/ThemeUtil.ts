import { Theme, createTheme } from "@mui/material";

export class ThemeUtil {


    static getTheme(): Theme {

        return createTheme({
            components: {
                MuiButtonBase: {
                    styleOverrides: {
                        root: {
                            color: 'var(--color-text)',
                        }
                    }
                }
            },
        });

    }

}