/*
     Canvas
 */

let parallax = (offset: int, plane_index: int) =>
  /* index: 0 = foreground (no transformation). index increases with "distance" */
  plane_index <= 0 ?
    offset :
    {
      /* let lag = (plane_index + 1 |> float_of_int) /. (plane_index |> float_of_int); */
      (offset |> float_of_int) /. (plane_index * plane_index |> float_of_int) |> Js.Math.floor
    };

let render = (~width, ~height, ~context: Dom_html.context, ~bgscroll: int, ()) => {
  context->(Dom_html.clearRect(0, 0, width, height));

  context->(CanvasBackgroundImage.render(CanvasData.bg, width, height, bgscroll->parallax(6)));
  context->(CanvasBackgroundImage.render(CanvasData.distantstreet, width, height, bgscroll->parallax(5)));
  context->(CanvasBackgroundImage.render(CanvasData.fgstreet, width, height, bgscroll->parallax(2)));
  context->(CanvasBackgroundImage.render(CanvasData.powerlines, width, height, bgscroll->parallax(1)));
  context->(CanvasBackgroundImage.render(CanvasData.pavement, width, height, bgscroll->parallax(0)));

  ();
};
