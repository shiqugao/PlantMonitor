#include <Arduino.h>
#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
#include <ESP_Mail_Client.h>

/* wifi ssid和password */
#define WIFI_SSID "SSID"
#define WIFI_PASSWORD "PASSWORD"

/* smtp address and port number of the qq mailbox */
#define SMTP_HOST "smtp.qq.com"
#define SMTP_PORT 465

/* 
Your email and authorization code The authorization code is a special password for logging in to third-party clients launched by QQ Mailbox.
How do I obtain an authorization code https://service.mail.qq.com/cgi-bin/help?subtype=1&&id=28&&no=1001256
*/
#define AUTHOR_EMAIL "xxxxxxx@qq.com"
#define AUTHOR_PASSWORD "xxxxxxxxx"
/* Email address of the recipient*/
#define RECIPIENT_EMAIL "xxxxxxxx@qq.com"

SMTPSession smtp;

ESP_Mail_Session session;

int tem =9;
/* A callback function that gets the message sending status */
void getSmtpStatusCallback(SMTP_Status status);


void wifiSetup(){
  Serial.println();
  Serial.print("connect Wifi");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(200);
  }
  Serial.println("");
  Serial.println("WiFi connect successfully.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();

}

void smtpSetup(){
   /* smtp enables debugging, and the debugging information is output to the serial monitor */
  smtp.debug(0);

  /* Register the callback function to get the message sending status */
  smtp.callback(getSmtpStatusCallback);

  /* Set smtp parameters, such as host and port */
  session.server.host_name = SMTP_HOST;
  session.server.port = SMTP_PORT;
  session.login.email = AUTHOR_EMAIL;
  session.login.password = AUTHOR_PASSWORD;
  session.login.user_domain = "";

  /* connect smtp server */
  if (!smtp.connect(&session))
    return;
}

/* Gets the callback function for the send status */
void getSmtpStatusCallback(SMTP_Status status){
  /* The email sending status is displayed */
  Serial.println(status.info());

  /*The status is successfully obtained, and the status information is printed */
  if (status.success()){
    Serial.println("----------------");
    
    ESP_MAIL_PRINTF("Number of successfully sent emails: %d\n", status.completedCount());
    ESP_MAIL_PRINTF("Number of failed email sending: %d\n", status.failedCount());
    
    Serial.println("----------------\n");
    struct tm dt;

    for (size_t i = 0; i < smtp.sendingResult.size(); i++){
      /* Obtain the status of outgoing emails in sequence */
      SMTP_Result result = smtp.sendingResult.getItem(i);
      time_t ts = (time_t)result.timestamp;
      localtime_r(&ts, &dt);
      ESP_MAIL_PRINTF("receiever: %sEmail sending status information\n", result.recipients);
      ESP_MAIL_PRINTF("state: %s\n", result.completed ? "success" : "failed");
      ESP_MAIL_PRINTF("send time: %d/%d/%d %d:%d:%d\n", dt.tm_year + 1900, dt.tm_mon + 1, dt.tm_mday, dt.tm_hour, dt.tm_min, dt.tm_sec);
      ESP_MAIL_PRINTF("email title: %s\n", result.subject);
    }
    
    Serial.println("----------------\n");
  }
}



void emailSendHtml(char* subject, char* recipient, String htmlMsg){
  
  /* Define the smtp message message class */
  SMTP_Message message;
    /* Define the mail message class name, sender, title and add recipient */
  message.sender.name = "Plant monitor";
  message.sender.email = AUTHOR_EMAIL;
  message.subject = subject;
  message.addRecipient("Joe", recipient);
  message.html.content = htmlMsg.c_str();
  message.html.content = htmlMsg.c_str();
  message.text.charSet = "us-ascii";
  message.html.transfer_encoding = Content_Transfer_Encoding::enc_7bit;
    /* Call the send mail function, if it fails, get the failure message */
  if (!MailClient.sendMail(&smtp, &message))
    Serial.println("Fail to send email and reason is , " + smtp.errorReason());
}

void setup(){
  /*Serial.begin(115200);*/
  wifiSetup();
  /*irSetup();
  */smtpSetup();
  
}

void loop(){
  if (tem<5) {
    String htmlMsg = "<div style=\"color:#2f4468;\"><h1>HELP!!!</h1><p>Send from Plant monitor</p></div>";
    emailSendHtml("Esp8266er Email", RECIPIENT_EMAIL, htmlMsg);
  }
}