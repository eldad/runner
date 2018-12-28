/*
   App View
 */

type action =
  | ColorHighlight(int)
  | ColorDeselect;

type state = {color_highlighted: option(int)};

let initialState = () => {color_highlighted: None};

let component = ReasonReact.reducerComponent("App");

let make = _children => {
  ...component,
  initialState,
  reducer: (action, _state: state) =>
    switch (action) {
    | ColorHighlight(i) => ReasonReact.Update({color_highlighted: Some(i)})
    | ColorDeselect => ReasonReact.Update({color_highlighted: None})
    },
  render: self =>
    <div style={ReactDOMRe.Style.make(~display="flex", ~justifyContent="center", ())}>
      <ColorBlocks highlight=?{self.state.color_highlighted} onBlockClick={i => self.send(ColorHighlight(i))} />
    </div>,
};
