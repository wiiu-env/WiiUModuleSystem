FROM ghcr.io/wiiu-env/devkitppc:20230621

WORKDIR build
COPY . .
RUN make clean && make && mkdir -p /artifacts/wums && cp -r lib /artifacts/wums && cp -r include /artifacts/wums && cp -r share /artifacts/wums
WORKDIR /artifacts

FROM scratch
COPY --from=0 /artifacts /artifacts