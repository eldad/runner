/*
     Canvas
 */

let render = (~width, ~height, ~context: Dom_html.context, ~points: int, ()) => {

  let size = points * 10;

  context->(Dom_html.clearRect(0, 0, width, height));
  context->(Dom_html.fillRect(0, 0, size, size));

  ();
};
