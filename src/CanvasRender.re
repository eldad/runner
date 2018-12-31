/*
     Canvas
 */

let render = (~width, ~height, ~context: Dom_html.context, ~points: int, ~image: Dom_html.img, ()) => {

  let size = points * 10;

  context->(Dom_html.clearRect(0, 0, width, height));
  context->(Dom_html.fillRect(0, 0, size, size));
  context->(Dom_html.drawImage(image, size, size));

  ();
};
