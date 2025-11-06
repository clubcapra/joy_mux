#ifndef JOY_MUX__JOY_MUX_HPP_
#define JOY_MUX__JOY_MUX_HPP_

#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/bool.hpp>
#include <sensor_msgs/msg/joy.hpp>
//#include <sensor_msgs/msg/joy_stamped.hpp> // n'existe pas

namespace joy_mux {
    //Forward declarations and definitions
    class JoyTopicHandler;
   // class JoyStampedTopicHandler


    class JoyMux : public rclcpp::Node {
        public:
            JoyMux();
            ~JoyMux() = default;
            template<typename T>
            using handle_container = std::list<T>;

            using joy_topic_container = handle_container<JoyTopicHandler>;
            //using joy_stamped_topic_container = handle_container<JoyStampedTopicHandler>;

            
            void  init();

            bool hasPriority(const JoyTopicHandler & joy);
            //bool hasPriorityStamped();

            void publishJoy(const sensor_msgs::msg::Joy::SharedPtr & msg);
            //void publishJoyStamped();

            //void updateDiagnostics(); //TODO: implement diagnostics
        
        private:

            std::shared_ptr<joy_topic_container> joy_hs_; //Joy topic handlers
            //std::shared_ptr<joy_stamped_topic_container> joy_stamped_hs_; // n'existe pas


            //TODO : verifier le type de message que joy_mux doit publier (pour l'instant c'est geometry_msgs/Twist)
            rclcpp::Publisher<sensor_msgs::msg::Joy>::SharedPtr joy_publisher_;
            //rclcpp::Publisher<geometry_msgs::msg::TwistStamped>::SharedPtr cmd_pub_stamped_; // n'existe pas
        
            sensor_msgs::msg::Joy last_status_;

            template<typename T>
            void getTopicHandles(const std::string & param_name, handle_container<T> & topic_hs);


    };


}  // namespace joy_mux


#endif  // JOY_MUX__JOY_MUX_HPP_