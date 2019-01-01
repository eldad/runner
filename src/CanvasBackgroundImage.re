/*
     Canvas: Background Image
 */

type t = {
  img: Dom_html.img,
  height: int,
  width: int,
};

let make = (~src: string, ~height, ~width, ()) => {img: Dom_html.createImgFromSrc(src), height, width};

let render = (context: Dom_html.context, bgimage: t, viewport_width: int, offset: int) => {
  let x1 = offset mod bgimage.width;
  let w1 = Js.Math.min_int(bgimage.width - x1, viewport_width);

  context->(Dom_html.blitImage(bgimage.img, x1, 0, 0, 0, w1, bgimage.height));
  if (w1 < viewport_width) {
    let w2 = viewport_width - w1;
    context->(Dom_html.blitImage(bgimage.img, 0, 0, w1, 0, w2, bgimage.height));
  }
};
