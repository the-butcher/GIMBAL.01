
export type TMethod = 'GET' | 'POST';
export interface IMethodParams<M extends TMethod> {
    method: M;
    allow: boolean;
}
// eslint-disable-next-line @typescript-eslint/no-empty-object-type
export interface IMethodParamsGet extends IMethodParams<'GET'> {

}
export interface IMethodParamsNoPost extends IMethodParams<'POST'> {
    allow: false;
}
export interface IMethodParamsPost extends IMethodParams<'POST'> {
    allow: true;
    body: unknown;
}

export interface ICcApiProps {
    uuid: string;
    title: string;
    ccApi: string;
    methods: [IMethodParamsGet, IMethodParamsNoPost | IMethodParamsPost];
}