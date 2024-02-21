DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
OUT_DIR="$DIR"
PROTO_PATH="$DIR"
PROTO_FILE="$DIR/paohuziProtocol.proto"

protoc --cpp_out="$OUT_DIR" --proto_path="$PROTO_PATH" "$PROTO_FILE"
