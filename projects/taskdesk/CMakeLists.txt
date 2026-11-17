add_executable(taskdesk main.cpp taskstore.h)
target_link_libraries(taskdesk PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Sql)
