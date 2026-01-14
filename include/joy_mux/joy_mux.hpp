#ifndef JOY_MUX__JOY_MUX_HPP_
#define JOY_MUX__JOY_MUX_HPP_

#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/bool.hpp>
#include <sensor_msgs/msg/joy.hpp>

#include <list>
#include <memory>
#include <string>

namespace joy_mux {
    //Forward declarations and definitions
    class JoyTopicHandler;
   // class JoyStampedTopicHandler //TODO: to implement if needed


    class JoyMux : public rclcpp::Node {
        public:
            template<typename T>
            using handle_container = std::list<T>;

            using joy_topic_container = handle_container<JoyTopicHandler>;
            //using lock_topic_container = handle_container<LockTopicHangler>; //TODO : to implement if needed

            JoyMux();
            ~JoyMux() = default;

            void  init();

            bool hasPriority(const JoyTopicHandler & joy);
            
            void publishJoy(const sensor_msgs::msg::Joy::SharedPtr & msg);

            //void updateDiagnostics(); //TODO: implement diagnostics if needed
        
        protected:

            std::shared_ptr<joy_topic_container> joy_hs_; //Joy topic handlers


            //TODO : verifier le type de message que joy_mux doit publier (pour l'instant c'est geometry_msgs/Twist)
            rclcpp::Publisher<sensor_msgs::msg::Joy>::SharedPtr joy_publisher_;
        
            sensor_msgs::msg::Joy last_status_;

            template<typename T>
            void getTopicHandles(const std::string & param_name, handle_container<T> & topic_hs);

            //int getLockPriority(); //TODO: implement lock topics if needed
    };


}  // namespace joy_mux


#endif  // JOY_MUX__JOY_MUX_HPP_