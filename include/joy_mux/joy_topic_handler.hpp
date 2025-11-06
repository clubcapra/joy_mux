#ifndef JOY_MUX__TPIC_HANDLER_HPP_
#define JOY_MUX__TPIC_HANDLER_HPP_

#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/bool.hpp>
#include <sensor_msgs/msg/joy.hpp>

#include <joy_mux/utils.hpp>
#include <joy_mux/joy_mux.hpp>

#include <memory>
#include <string>

namespace joy_mux{
    template<typename T>
    class TopicHandler_{
        public :
            //Singleton
            TopicHandler_(TopicHandler_ &) = delete;
            TopicHandler_(const TopicHandler_ &) = delete;

            //To not copy assignable
            TopicHandler_ & operator=(TopicHandler_ &) = delete;
            TopicHandler_ & operator=(const TopicHandler_ &) = delete;
        
            typedef int priority_type;
            TopicHandler_(const std::string & name, const std::string & topic, 
                        const rclcpp::Duration & timeout, priority_type priority, 
                        JoyMux * mux):
                name_(name),
                topic_(topic),
                timeout_(timeout),
                priority_(clamp(priority, priority_type(0), priority_type(255))),
                mux_(mux),
                stamp_(0) {
                    RCLCPP_INFO(
                        mux_->get_logger(),
                        "Created TopicHandler '%s' subscribed to topic '%s': timeout = '%.2f', priority %d",
                        name_.c_str(),
                        topic_.c_str(),
                        ((timeout_.seconds()>0) ? std::to_string(timeout_.seconds()) + "s" : "None").c_str(),
                        static_cast<int>(priority_)
                    );
                }

            virtual ~TopicHandler_() = default;

            bool hasExpired() const{
                return (timeout_.seconds() > 0.0) &&
                        ((mux_->now().seconds() - stamp_.seconds()) > timeout_.seconds());
            }

            const std::string & getName() const{
                return name_;
            }

            const std::string & getTopic() const{
                return topic_;
            }

            const rclcpp::Duration & getTimeout() const{
                return timeout_;
            }

            const priority_type & getPriority() const{
                return priority_;
            }

            const T & getStamp() const{
                return stamp_;
            }
            const T & getMessage() const{
                return msg_;
            }
        
        protected:
            //Topic config
            std::string name_;
            std::string topic_;
            typename rclcpp::Subscription<T>::SharedPtr subscriber_;
            rclcpp::Duration timeout_;
            priority_type priority_;
            
            //TODO : pas sur ils font quoi
            JoyMux * mux_;
            rclcpp::Time stamp_;
            T msg_;
    };

    class JoyTopicHandler : public TopicHandler_<sensor_msgs::msg::Joy>{
        private:
            typedef TopicHandler_<sensor_msgs::msg::Joy> base_type;

        public:
            typedef typename base_type::priority_type priority_type;

            //Constructor
            JoyTopicHandler( const std::string & name, const std::string & topic, 
                            const rclcpp::Duration & timeout, priority_type priority, JoyMux * mux)
                : base_type(name, topic, timeout, priority, mux){
                    //Create subscriber
                    subscriber_ = mux_->create_subscription<sensor_msgs::msg::Joy>(
                        topic_,
                        rclcpp::SystemDefaultsQoS(),//TODO Q: ca fait quoi?
                        std::bind(&JoyTopicHandler::callback, this, std::placeholders::_1)//TODO Q: ca fait quoi?
                    );
                }
            
            void callback(const sensor_msgs::msg::Joy::SharedPtr msg){//TODO Q: pourquoi c'est appelé callback() et non publish()?
                stamp_ = mux_->now();//TODO Q : pas sur de ca fait quoi
                msg_ = *msg;

                // Verify that the message has the higehst priority
                if(mux_->hasPriority(*this)){ 
                    mux_->publishJoy(msg);
                }
            }

    };
}

#endif  // JOY_MUX__TPIC_HANDLER_HPP_