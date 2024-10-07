@echo off
set GRPC_PLUGIN_PATH=D:\Projects\git\vcpkg\packages\grpc_x64-windows\tools\grpc\grpc_cpp_plugin.exe
set PROTO_FILE=message.proto

echo Generating gRPC code...
protoc -I="." --grpc_out="." --plugin=protoc-gen-grpc="%GRPC_PLUGIN_PATH%" "%PROTO_FILE%"

echo Generating C++ code...
protoc --cpp_out=. "%PROTO_FILE%"

echo Done.
pause