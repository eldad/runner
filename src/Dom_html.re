/*
     DOM Bindings and helper functions
 */

type document;
let document: document = [%bs.raw {| document |}];

/* Animation frame */

type requestId;
[@bs.val] external requestAnimationFrame: (float => unit) => requestId = "";
[@bs.val] external cancelAnimationFrame: requestId => unit = "";

/* Keyboard events (document level) */

[@bs.send] external addEventListener: (document, string, Dom.event_like('a) => bool, bool) => unit = "";
[@bs.send] external removeEventListener: (document, string, Dom.event_like('a) => bool, bool) => unit = "";
external keyboardEventToJsObj: Dom.keyboardEvent => Js.t({..}) = "%identity";

let keyboardEventKey: Dom.keyboardEvent => string = keyboardEvent => keyboardEvent->keyboardEventToJsObj##key;
let keyboardEventIsRepeat: Dom.keyboardEvent => bool = keyboardEvent => keyboardEvent->keyboardEventToJsObj##repeat;

/* Canvas */

type canvas;
type element;
type context;

[@bs.send] external getContext: (canvas, string) => context = "";
[@bs.send] external getCanvasById: (document, string) => Js.Nullable.t(canvas) = "getElementById";
[@bs.send] external getElementById: (document, string) => Js.Nullable.t(element) = "getElementById";

let getCanvas2DContext: string => option(context) =
  (canvasId: string) =>
    canvasId |> getCanvasById(document) |> Js.Nullable.toOption |> Js.Option.map((. c) => getContext(c, "2d"));

/* Canvas Ops */

external js_of_canvas: canvas => Js.t({..}) = "%identity";
external js_of_context: context => Js.t({..}) = "%identity";

[@bs.send] external clearRect: (context, int, int, int, int) => unit = "";
[@bs.send] external fillRect: (context, int, int, int, int) => unit = "";

let fillStyle = (context, fillStyle: string) => (context |> js_of_context)##fillStyle #= fillStyle;

/* Canvas Text */

let setFont = (context, font: string) => (context |> js_of_context)##font #= font;

type text_metrics;

[@bs.send] external fillText: (context, string, int, int) => unit = "";
[@bs.send] external fillTextMaxWidth: (context, string, int, int) => unit = "fillText";

[@bs.send] external measureText: (context, string) => text_metrics = "";
external js_of_text_metrics: text_metrics => Js.t({..}) = "%identity";

let measureTextWidth = (context, text) => (measureText(context, text) |> js_of_text_metrics)##width;

/* Image */

type img;
[@bs.send] external createImg: (document, [@bs.as "img"] _) => img = "createElement";
external js_of_img: img => Js.t({..}) = "%identity";

let createImgFromSrc: string => img =
  src => {
    let img = createImg(document);
    (img |> js_of_img)##src #= src;
    img;
  };

/* void ctx.drawImage(image, dx, dy); */
[@bs.send] external drawImage: (context, img, int, int) => unit = "";

/* void ctx.drawImage(image, sx, sy, sWidth, sHeight, dx, dy, dWidth, dHeight); */
[@bs.send] external drawImageBlit: (context, img, int, int, int, int, int, int, int, int) => unit = "drawImage";

[@bs.send] external requestFullscreen: element => unit = "";
[@bs.send] external webkitRequestFullScreen: element => unit = "";

let blitImage = (context, img, sx, sy, dx, dy, w, h) => drawImageBlit(context, img, sx, sy, w, h, dx, dy, w, h);

let requestFullscreenElement = elementId =>
  elementId
  |> getElementById(document)
  |> Js.Nullable.toOption
  |> Js.Option.andThen((. c) => {
       let error = ref(false);
       try (requestFullscreen(c)) {
       | _ => error := true
       };

       if (error^) {
         Js.log("trying webkitRequestFullScreen");
         try (webkitRequestFullScreen(c)) {
         | _ => ()
         };
       };

       None;
     });
