set(LIBLIST
    KA_UI
)

foreach(LIB ${LIBLIST})
    add_subdirectory(${LIB})
endforeach(LIB ${LIBLIST})

set(LIBLIST ${LIBLIST} PARENT_SCOPE)
