
# syntax=docker/dockerfile:1

# 1. native image
FROM --platform=$BUILDPLATFORM osxcross:0.13.1 AS cross
ARG TARGETPLATFORM
ARG TARGETOS
ARG TARGETARCH

# 2. Set environment variables for cross-compilation
RUN echo "Building for $TARGETPLATFORM"
FROM nomlib:0.13.1 AS build
WORKDIR /nomlib

# Compile for target
#RUN cmake ..
#RUN GOOS=$TARGETOS GOARCH=$TARGETARCH go build -o myapp .

# 3. Final Stage: Runtime image
#FROM alpine:latest
#WORKDIR /nomlib
# Copy binary from build stage
#COPY --from=build /nomlib/build /tmp
#CMD ["examples/Debug/device_info"]

