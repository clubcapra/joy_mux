#include "joy_mux/joy_mux.hpp"
#include "joy_mux/joy_topic_handler.hpp"
#include "joy_mux/params_helpers.hpp"

namespace joy_mux {

    // constexpr std::chrono::duration<int64_t> JoyMux::DIAGNOSTICS_PERIOD;

    JoyMux::JoyMux()
        : Node("joy_mux", "", rclcpp::NodeOptions() /*.allow_underclared_parameters(true).automatically_declare_parameters_from_ovverides(true)*/) 
        {}

    void JoyMux::init() {
        RCLCPP_INFO(get_logger(), "Initializing Joy Mux Node");
        //TODO : est-ce necessaire? on ignore (a chancher plus tard)
        bool use_stamped = true;
        this->declare_parameter("use_stamped", use_stamped);

        auto nh = std::shared_ptr<rclcpp::Node>(this, [](rclcpp::Node *) {});
        fetch_param(nh, "use_stamped", use_stamped);

        //if(use_stamped){} // n'existe pas de joy_stamped, doit creer un joy_stamped?

        //Get topic
        joy_hs_ = std::make_shared<joy_topic_container>();
        getTopicHandles("joy_topics", *joy_hs_);

        //Publisher for output topic
        joy_publisher_ = this->create_publisher<sensor_msgs::msg::Joy>("joy_mux_topic",rclcpp::QoS(rclcpp::KeepLast(1)));//TODO: verifier le nom du topic
    }

    bool JoyMux::hasPriority(const JoyTopicHandler & joy) {
        std::string joy_name = "NULL";
        JoyTopicHandler::priority_type priority = -1;

        //Find the highest priority joy
        for(const auto & joy_h : *joy_hs_){
            if(joy_h.getPriority() < joy.getPriority()){
                priority = joy_h.getPriority();
                joy_name = joy_h.getName();
            }
        }

        return joy.getName() == joy_name;
    }

    void JoyMux::publishJoy(const sensor_msgs::msg::Joy::SharedPtr & msg) {
        joy_publisher_->publish(*msg);
    }

    template<typename T>
    void JoyMux::getTopicHandles(const std::string & param_name, handle_container<T> & topic_hs) {
        RCLCPP_DEBUG(get_logger(),"Prefix %s",param_name.c_str());

        rcl_interfaces::msg::ListParametersResult list = list_parameters({param_name},10);

        try{
            for(auto prefix : list.prefixes){
                RCLCPP_DEBUG(get_logger(),"Found prefix %s",prefix.c_str());
                
                std::string topic;
                double timeout = 0;
                int priority = 0;

                auto nh = std::shared_ptr<rclcpp::Node>(this, [](rclcpp::Node *) {});

                fetch_param(nh, prefix + ".topic", topic);
                fetch_param(nh, prefix + ".timeout", timeout);
                fetch_param(nh, prefix + ".priority", priority);

                RCLCPP_DEBUG(get_logger(),"Retrieved topic: %s", topic.c_str());
                RCLCPP_DEBUG(get_logger(),"Listed prefix: %.2f", timeout);
                RCLCPP_DEBUG(get_logger(),"Listed priority: %d", priority);

                topic_hs.emplace_back(prefix, topic, std::chrono::duration<double>(timeout), priority, this);  
            }
        } catch (const ParamsHelperException & e){
        RCLCPP_FATAL(get_logger(), "Error parsing params '%s'\n\t%s", param_name.c_str(), e.what());
            throw e;
        }
    }
}  // namespace joy_mux