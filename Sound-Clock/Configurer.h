
class Configurer {

  String filename;
  boolean SaveSettings(const char *settings);
  
public:
	Configurer();
	boolean GetConfig(String &ssid, String &password);
	void setup();
	void loop();
};
