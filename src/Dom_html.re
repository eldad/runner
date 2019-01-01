/*
     DOM Bindings and helper functions
 */

/* Animation frame */

type requestId;
[@bs.val] external requestAnimationFrame: (float => unit) => requestId = "";
[@bs.val] external cancelAnimationFrame: requestId => unit = "";

/* Canvas */

type canvas;
type element;
type context;
type document;

let document: document = [%bs.raw {| document |}];

[@bs.send] external getContext: (canvas, string) => context = "";
[@bs.send] external getCanvasById: (document, string) => Js.Nullable.t(canvas) = "getElementById";
[@bs.send] external getElementById: (document, string) => Js.Nullable.t(element) = "getElementById";

let getCanvas2DContext: string => option(context) =
  (canvasId: string) =>
    canvasId |> getCanvasById(document) |> Js.Nullable.toOption |> Js.Option.map((. c) => getContext(c, "2d"));

/* Canvas Ops */

[@bs.send] external clearRect: (context, int, int, int, int) => unit = "";
[@bs.send] external fillRect: (context, int, int, int, int) => unit = "";

/* Escape Hatch: use canvas as Js option and access members with ## */

external js_of_canvas: canvas => Js.t({..}) = "%identity";

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

let blitImage = (context, img, sx, sy, dx, dy, w, h) => {
  drawImageBlit(context, img, sx, sy, w, h, dx, dy, w, h);
};

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
