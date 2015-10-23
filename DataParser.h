#include<Arduino.h>

#define MAGIC_CHAR '$'

#define MAX_PASSWORD_LEN 10
#define MAX_PHONE_LEN 10
/*
 * Workflow
 * --------
 *  1) Text message to required data
 *
 *  Give DataParser a chunk of text
 *  it will return an instance of Request_Message
 *  whose message_type field indicates
 *  whether the message failed to parse
 *  or if it succeed, what type of message it is
 *  See REQ_XXXX definitions for message types
 *
 *  2) Data to message
 *
 *  Currently the parser can accept an instance of
 *  Status_Message_To
 */

/*
 * These constants indicates what
 * type of request an instance of
 * Request_Message have
 * See also private REQ_MSG_TYPE
 */

#define REQ_INVALID  255
#define REQ_STATUS 0
#define REQ_UPDATE_STATUS 1
#define REQ_CHANGE_PASSWORD 2
#define REQ_CHANGE_PHONE 3

#define REQ_STATUS_TOKEN "STATUS"
#define REQ_UPDATE_STATUS_TOKEN "UPDATE"
#define REQ_CHANGE_PASSWORD_TOKEN "CPW"
#define REQ_CHANGE_PHONE_TOKEN "CPHONE"



#define CODE_STATUS_UPDATED 200
#define CODE_PASSWORD_CHANGED 201
#define CODE_PHONE_CHANGED  202

class DataParser {

  public:
    /*
     * After parsing an incoming
     * message, DataParser will populate
     * given instance of this struct
     */
    struct Status_Message_To_Phone {
      byte device_on_off; //LSB -> device 1
      byte temperature;
      byte waterlevel_percent;
    };
    /*
     * All the request messages will contain
     * an instance of this struct
     * The external authenticator is supposed
     * to check its contents and discard rest
     * of the message if failed
     */
    struct Auth_Data {
      char password[MAX_PASSWORD_LEN];
    };
    /*
     * Holds the data for updating the status
     * Currently,new device status only
     */
    struct Status_Update_Request {
      byte device_on_off; //first device is the right most
      byte device_on_off_mask;
    };
    /*
     * Change password request message
     * contains new password
     */
    struct Change_Password_Request {
      char new_password[MAX_PASSWORD_LEN];
    };
    /*
     * Change phoneno request message
     * contains new phoneno
     */
    struct Change_Phone_Request {
      char new_phone[MAX_PHONE_LEN];
    };
    /*
     * A struct which contains
     * auth data
     * a byte indicating what type of request do we have
     * and a union of all the request messages
     * the coder is supposed to check the byte first and read
     * appropriate union-variable
     */
    struct Request_Message {
      byte message_type;
      Auth_Data auth_data;
      union {
        Status_Update_Request status_update_request;
        Change_Password_Request change_password_request;
        Change_Phone_Request change_phone_request;
      };
    };
    /*
     * will try to parse the request message
     * and populate the request_message variable
     * passed as reference appropriately
     * if parsing failed, the request_message's
     * message_type will be set to invalid(see #define section)
     */
    void parseRequest(char message[160],Request_Message& request_message);
    void parseStatusReply(char message[160],Status_Message_To_Phone& status);
  private:

};
