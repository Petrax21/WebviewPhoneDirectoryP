cmake_minimum_required(VERSION 3.28)
project(rabbitmq_project)

set(CMAKE_CXX_STANDARD 20)

# RabbitMQ-C kütüphanesini ekle
find_package(PkgConfig REQUIRED)
pkg_check_modules(RABBITMQ REQUIRED librabbitmq)

include_directories(${RABBITMQ_INCLUDE_DIRS})
link_directories(${RABBITMQ_LIBRARY_DIRS})

add_executable(rabbitmq_contact_app consumer.cpp consumer.h producer.cpp producer.h)

# RabbitMQ-C ile bağlantı
target_link_libraries(rabbitmq_contact_app ${RABBITMQ_LIBRARIES})

