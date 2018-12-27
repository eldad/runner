/*
     Color Blocks
 */

let colors = [|"#00A878", "#D8F1A0", "#F3C178", "#FE5E41"|];

let blocksize = "100px";

let component = ReasonReact.statelessComponent("ColorBlocks");

let make = _children => {
  ...component,
  render: _self =>
    <div>
      {
        colors
        |> Array.mapi((i, c) =>
             <div
               key={i |> string_of_int}
               style={ReactDOMRe.Style.make(~backgroundColor=c, ~width=blocksize, ~height=blocksize, ())}
             />
           )
        |> ReasonReact.array
      }
    </div>,
};
