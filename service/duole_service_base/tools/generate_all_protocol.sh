# 此脚本用于编译指定目录下所有的 proto 文件，支持子目录；

# 指定 proto 文件根目录；
PROTO_ROOT='../service'

# 遍历 proto 目录
for i in `find ${PROTO_ROOT} -name 'proto' -type d`; do
	#statements
	echo ${i}
	# 清理上一次编译结果
	rm ${i}/*.pb.cc ${i}/*.pb.h

	# protoc 编译
	for j in `find ${i} -name '*.proto'`; do
		#statements
		echo ${j}
		protoc -I ${i} --cpp_out ${i} ${j}
	done	
done
