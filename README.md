# Runner

A simple runner game written using ReasonReact.

Play it [here!](https://eldad.github.io/runner/)

Features:
* Amazing parallax effect
* Continuous stream of crates to jump above
* Highscore tracking
* Available offline (via service worker)

Pixel art by [Jason Mercado (GrafixKid)](https://opengameart.org/users/grafxkid) - OpenGameArt.org

# Development

Run it locally:
```
$ yarn
$ make start
```
Then point your browser to http://localhost:3000.

# Known Issues

Fullscreen request is best effort. Instead, its promise should be resolved and the app state updated.
