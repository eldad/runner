/*
     Color Blocks Animation
 */

let timeout_start = 700;
let timeout_next = 100;
let timeout_pause = 700;

type state = {
  position: int,
  highlighting: bool,
};

let initialState = () => {position: 0, highlighting: false};

type action =
  | Start
  | Pause
  | Next;

let component = ReasonReact.reducerComponent("ColorBlocksAnimation");

let make = (~sequence, ~onFinish, _children) => {
  ...component,
  initialState,
  didMount: self => {
    let intervalId = Js.Global.setTimeout(() => self.send(Start), timeout_start);
    self.onUnmount(() => Js.Global.clearTimeout(intervalId));
  },
  reducer: (action, state: state) =>
    switch (action) {
    | Start =>
      ReasonReact.UpdateWithSideEffects(
        {position: 0, highlighting: true},
        (
          self => {
            let intervalId = Js.Global.setTimeout(() => self.send(Pause), timeout_pause);
            self.onUnmount(() => Js.Global.clearTimeout(intervalId));
          }
        ),
      )
    | Pause =>
      ReasonReact.UpdateWithSideEffects(
        {...state, highlighting: false},
        (
          self => {
            let intervalId = Js.Global.setTimeout(() => self.send(Next), timeout_next);
            self.onUnmount(() => Js.Global.clearTimeout(intervalId));
          }
        ),
      )
    | Next =>
      ReasonReact.UpdateWithSideEffects(
        {position: state.position + 1, highlighting: true},
        (
          self =>
            if (self.state.position > Array.length(sequence)) {
              onFinish();
            } else {
              let intervalId = Js.Global.setTimeout(() => self.send(Pause), timeout_pause);
              self.onUnmount(() => Js.Global.clearTimeout(intervalId));
            }
        ),
      )
    },
  render: self =>
    <ColorBlocks
      highlight=?{
        self.state.highlighting && self.state.position < Array.length(sequence) ? Some(sequence[self.state.position]) : None
      }
    />,
};
