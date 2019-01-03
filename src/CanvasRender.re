/*
     Canvas
 */

let parallax = (offset: int, plane_index: int) =>
  /* index: 0 = foreground (no transformation). index increases with "distance" */
  plane_index <= 0 ?
    offset :
    {
      let lagfactor = plane_index * plane_index |> float_of_int;
      (offset |> float_of_int) /. lagfactor |> Js.Math.floor;
    };

let render = (~width, ~height, ~context: Dom_html.context, ~data: GameState.t, ()) => {
  context->(Dom_html.clearRect(0, 0, width, height));

  let bgscroll = data.bgscroll |> int_of_float;

  context->(CanvasBackgroundImage.render(CanvasData.bg, width, height, bgscroll->parallax(6)));
  context->(CanvasBackgroundImage.render(CanvasData.distantstreet, width, height, bgscroll->parallax(5)));
  context->(CanvasBackgroundImage.render(CanvasData.fgstreet, width, height, bgscroll->parallax(2)));
  context->(CanvasBackgroundImage.render(CanvasData.powerlines, width, height, bgscroll->parallax(1)));
  context->(CanvasBackgroundImage.render(CanvasData.pavement, width, height, bgscroll->parallax(0)));

  let player_size = 10;
  let player_y = height - player_size - (data.player_y |> Js.Math.floor);

  context->Dom_html.fillRect(0, player_y, player_size, player_size);

  ();
};
