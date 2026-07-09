function(sy2ools_target target)
    mco_target(${target})
    target_include_directories(${target} PUBLIC ${PROJECT_SOURCE_DIR}/include)
endfunction()
