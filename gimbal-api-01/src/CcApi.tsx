import ExpandMoreIcon from '@mui/icons-material/ExpandMore';
import { Accordion, AccordionDetails, AccordionSummary, Button, Divider, Stack, Typography } from '@mui/material';
import { useState } from 'react';
import type { ICcApiProps, TMethod } from './ICcApiProps';


function CcApi(props: ICcApiProps) {

    const { uuid, title, ccApi, methods } = { ...props };

    const methodGet = methods.find(m => m.method === 'GET')!;
    const methodPost = methods.find(m => m.method === 'POST')!;

    const [response, setResponse] = useState<string | undefined>();
    const [expanded, setExpanded] = useState<string | false>(false);
    const handleChange = (panel: string) => (_event: React.SyntheticEvent, isExpanded: boolean) => {
        setExpanded(isExpanded ? panel : false);
    };

    const issueCcApiRequest = async (method: TMethod): Promise<void> => {

        const headers: Headers = new Headers()
        headers.set('Content-Type', 'text/plain')
        headers.set('Accept', 'application/json')

        const request: RequestInfo = new Request(ccApi, {
            method,
            headers: headers,
            body: method == 'POST' && methodPost.allow ? JSON.stringify(methodPost.body) : undefined
        });

        const timeoutMs = 5000;
        const controller = new AbortController();
        const timeoutTo = setTimeout(() => {
            controller.abort("timeout");
            console.log('request has timed out');
        }, timeoutMs);

        fetch(request, {
            signal: controller.signal
        }).then(res => res.json()).then(res => {

            console.log('res', res);

            setResponse(JSON.stringify(res, null, 2));
            clearTimeout(timeoutTo);

        }).catch((e: Error) => {

            console.warn(e);
            setResponse(String(e));

        });

    }

    return (

        <Accordion
            expanded={expanded === uuid}
            onChange={handleChange(uuid)}
        >
            <AccordionSummary
                expandIcon={<ExpandMoreIcon />}
                aria-controls={`${uuid}-content`}
                id={`${uuid}-header`}
            >
                <Stack
                    direction={'row'}
                    sx={{
                        display: 'flex',
                        alignItems: 'baseline'
                    }}
                >
                    <Typography variant='h5'>{title}&nbsp;</Typography><Typography variant='caption'>{ccApi}</Typography>
                </Stack>

            </AccordionSummary>
            <AccordionDetails>
                <Stack
                    direction={'column'}
                    spacing={1}
                >
                    {
                        methodGet.allow ? <Stack
                            direction={'row'}
                            sx={{
                                display: 'flex',
                                alignItems: 'center'
                            }}
                        >
                            <Typography
                                sx={{
                                    flexGrow: 1
                                }}
                            ></Typography>
                            <Button
                                variant={'outlined'}
                                sx={{
                                    width: '120px'
                                }}
                                onClick={() => issueCcApiRequest('GET')}
                            >GET</Button>
                        </Stack> : null
                    }
                    {
                        methodPost.allow ? <Stack
                            direction={'row'}
                            sx={{
                                display: 'flex',
                                alignItems: 'center'
                            }}
                        >
                            <Typography
                                sx={{
                                    minWidth: '150px',
                                    fontFamily: 'Consolas'
                                }}
                            >
                                body:
                            </Typography>
                            <Typography
                                sx={{
                                    flexGrow: 1,
                                    fontFamily: 'Consolas'
                                }}
                            >
                                {JSON.stringify(methodPost.body, null, 2)}
                            </Typography>
                            <Button
                                variant={'outlined'}
                                sx={{
                                    width: '120px'
                                }}
                                onClick={() => issueCcApiRequest('POST')}
                            >POST</Button>
                        </Stack> : null

                    }
                    {
                        response ? <>
                            <Divider></Divider>
                            <Stack
                                direction={'row'}
                                sx={{
                                    display: 'flex',
                                    alignItems: 'center'
                                }}
                            >
                                <Typography
                                    sx={{
                                        minWidth: '150px',
                                        fontFamily: 'Consolas'
                                    }}
                                >
                                    response:
                                </Typography>
                                <Typography
                                    sx={{
                                        flexGrow: 1,
                                        fontFamily: 'Consolas'
                                    }}
                                >{response}</Typography>
                            </Stack>
                        </> : null
                    }

                </Stack>
            </AccordionDetails>

        </Accordion >

    )
}

export default CcApi
