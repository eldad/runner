/*
    DOM Bindings and helper functions
*/

/* Animation frame */

type requestId;
[@bs.val] external requestAnimationFrame : (float => unit) => requestId = "";
[@bs.val] external cancelAnimationFrame : requestId => unit = "";

/* Canvas */

type canvas;
type context;
type document;

let document: document = [%bs.raw {| document |}];

[@bs.send] external getContext : (canvas, string) => context = "";
[@bs.send] external getCanvasById : (document, string) => canvas = "getElementById";

let getCanvas2DContext: string => context = (canvasId: string) => {
    let canvas = getCanvasById(document, canvasId);
    getContext(canvas, "2d");
};

/* Canvas Ops */

[@bs.send] external clearRect : (context, int, int, int, int) => unit = "";
[@bs.send] external fillRect : (context, int, int, int, int) => unit = "";

/* Escape Hatch: use canvas as Js option and access members with ## */

external js_of_canvas : context => Js.t({..}) = "%identity";
