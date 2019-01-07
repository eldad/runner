# Targets:
# `all` (default): build production bundle from a clean directory.
# 'start':         start development server (without clearing the screen)
# 'clean':         clean up generated files
# 'bs':            make bucklescript

.PHONY: build deps root clean remake build start bs public/media favicon
build: deps
	yarn build

deps: public root public/media favicon

remake: clean all

clean:
	rm -r public

public:
	mkdir -p public

root: public/index.html public/manifest.json public/favicon.ico

public/media:
	rsync -ra --delete --exclude CREDITS root/media/ public/media/

public/index.html: root/index.html
	cp root/index.html public/

public/manifest.json: root/manifest.json
	cp root/manifest.json public/

favicon: root/favicon/*
	cp root/favicon/* public/

start: deps
	# https://github.com/facebook/create-react-app/issues/2495
	FORCE_COLOR=true yarn start | cat

bs:
	yarn bsb -make-world

refmt:
	find src -type f -name '*.re' | xargs refmt --in-place -w 132
