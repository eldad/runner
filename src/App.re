/*
   App View
 */

let generate_sequence = () => {
  Array.make(5, 0) |> Array.map((_v) => Random.int(4));
};

/* Use polymorphic variants, because they get converted to strings */
[@bs.deriving jsConverter]
type stage = [ | `Idle | `Play | `Input ];

type action =
  | ColorHighlight(int)
  | ColorDeselect
  | Play;

type state = {
  color_highlighted: option(int),
  stage,
  sequence: array(int),
};

let initialState = () => {color_highlighted: None, stage: `Idle, sequence: [||]};

let component = ReasonReact.reducerComponent("App");

let make = _children => {
  ...component,
  initialState,
  reducer: (action, state) =>
    switch (action) {
    | ColorHighlight(i) => ReasonReact.Update({...state, color_highlighted: Some(i)})
    | ColorDeselect => ReasonReact.Update({...state, color_highlighted: None})
    | Play when state.stage == `Idle => ReasonReact.Update({...state, sequence: generate_sequence(), stage: `Play})
    | _ => ReasonReact.NoUpdate
    },
  render: self =>
    <div className=Glamor.(css([display("flex"), alignItems("center"), flexDirection("column")]))>
      <div className=Glamor.(css([backgroundColor("#000")]))> {self.state.stage |> stageToJs |> ReasonReact.string} </div>
      {switch (self.state.stage) {
      | `Idle =>
      <div>
      <button onClick=((_e) => self.send(Play))>
        {"Play" |> ReasonReact.string}
      </button>
      <ColorBlocks highlight=?self.state.color_highlighted onBlockClick={i => self.send(ColorHighlight(i))} />
      </div>
      | `Play =>
      ReasonReact.null
      | `Input =>
      ReasonReact.null
      }}
    </div>,
};
