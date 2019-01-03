/*
     Canvas Sprite
 */

type align =
  | Top
  | Bottom;

type t = {
  img: Dom_html.img,
  height: int,
  width: int,
  frames: int,
  align,
};

let make = (~src: string, ~height, ~width, ~frames, ~align=Bottom, ()) => {
  img: Dom_html.createImgFromSrc(src),
  height,
  width,
  frames,
  align,
};

let render = (~viewport_h=?, context: Dom_html.context, sprite: t, x: int, y: int, frame: int) => {
  let sx = sprite.width * (frame mod sprite.frames);

  let dy = switch (sprite.align, viewport_h) {
  | (Bottom, Some(h)) => h - y - sprite.height;
  | _ => y;
  }

  context->(Dom_html.blitImage(sprite.img, sx, 0, x, dy, sprite.width, sprite.height));
};
