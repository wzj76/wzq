DIR=$(cd $(dirname $0) && pwd)
rm *.cc *.h
for i in `find ./ -name '*.proto'`;do
    echo ${i}
    protoc -I ./ --cpp_out ./ ${i}
done
