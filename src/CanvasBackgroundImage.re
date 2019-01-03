/*
     Canvas: Background Image
 */

type align =
  | Top
  | Bottom;

type t = {
  img: Dom_html.img,
  height: int,
  width: int,
  align,
};

let make = (~src: string, ~height, ~width, ~align=Top, ()) => {img: Dom_html.createImgFromSrc(src), height, width, align};

let render = (context: Dom_html.context, bgimage: t, viewport_width: int, viewport_height: int, offset: int) => {
  let dy =
    switch (bgimage.align) {
    | Top => 0
    | Bottom => viewport_height - bgimage.height
    };

  /* mod operation may return negative numbers */
  let sx = ref(offset mod bgimage.width);
  if (sx^ < 0) {
    sx := sx^ + bgimage.width;
  };

  let sw = Js.Math.min_int(bgimage.width - sx^, viewport_width);

  context->(Dom_html.blitImage(bgimage.img, sx^, 0, 0, dy, sw, bgimage.height));

  let dx = ref(sw);

  while (dx^ < viewport_width) {
    let w = Js.Math.min_int(viewport_width - dx^, bgimage.width);
    context->(Dom_html.blitImage(bgimage.img, 0, 0, dx^, dy, w, bgimage.height));

    dx := dx^ + bgimage.width;
  };
};
