FIND_PACKAGE(Boost COMPONENTS system filesystem REQUIRED)


set ( testPutter_OBJ testPutter )
ADD_EXECUTABLE ( ${testPutter_OBJ} testPutter.cpp )
TARGET_LINK_LIBRARIES( ${testPutter_OBJ} 
    rtdb 
    rtdb2 
    pthread 
    ${Boost_FILESYSTEM_LIBRARY} 
    ${Boost_SYSTEM_LIBRARY}
)

set ( testGetter_OBJ testGetter )
ADD_EXECUTABLE ( ${testGetter_OBJ} testGetter.cpp )
TARGET_LINK_LIBRARIES( ${testGetter_OBJ} 
    rtdb 
    rtdb2 
    pthread 
    ${Boost_FILESYSTEM_LIBRARY} 
    ${Boost_SYSTEM_LIBRARY}
)



