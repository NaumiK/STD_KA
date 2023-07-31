set(APPLIST
    STD_APP
    animationView
)

foreach(APP ${APPLIST})
    add_subdirectory(${APP})
endforeach(APP ${APPLIST})

set(APPLIST ${APPLIST} PARENT_SCOPE)
