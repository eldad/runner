# Colorpeat


Issues:

Fullscreen request is best effort. Instead, its promise should be resolved and the app state updated.

Canvas mouse click event (offsetX, offsetY) in fullscreen mode is "wrong" (it is actually the screen resolution.)
-> Fullscreen request promise should be resolved. The offsets then need to be scaled.
