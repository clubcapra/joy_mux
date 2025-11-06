#include "joy_mux/joy_mux.hpp"
#include <memory>

//TODO Q : pourquoi est-ce que le main n'est pas dans joyt_mux.hpp? et explications sur l'utilite/fonctionnement du main
//TODO Q : comment je le test?
int main(int argc, char ** argv) {
    rclcpp::init(argc, argv);

    auto joy_mux_node = std::make_shared<joy_mux::JoyMux>();
    
    joy_mux_node->init();

    rclcpp::spin(joy_mux_node);
    
    rclcpp::shutdown();
    
    return EXIT_SUCCESS;
}