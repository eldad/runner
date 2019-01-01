/*
     Canvas
 */

let render = (~width, ~height, ~context: Dom_html.context, ~bg_image: CanvasBackgroundImage.t, ~bgscroll: int, ()) => {

  context->(Dom_html.clearRect(0, 0, width, height));
  context->(CanvasBackgroundImage.render(bg_image, width, bgscroll));

  ();
};
