include(CMakeParseArguments)
# 递归添加头文件
function(include_sub_directories_recursively root_dir)
    # 如果当前路径是目录，就加入到包含目录
    if (IS_DIRECTORY ${root_dir})               
        message("include dir: " ${root_dir})
        include_directories(${root_dir})
    endif()
    # 获得当前目录下的所有文件及子目录，加入ALL_SUB列表中
    file(GLOB ALL_SUB RELATIVE ${root_dir} ${root_dir}/*) 
    foreach(sub ${ALL_SUB})
        if (IS_DIRECTORY ${root_dir}/${sub})
            # 对子目录递归调用，包含
            include_sub_directories_recursively(${root_dir}/${sub}) 
        endif()
    endforeach()
endfunction()

