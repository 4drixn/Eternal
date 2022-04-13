
class c_AutoSDK {
private:
	HWND window = nullptr;
	std::string window_text;

public:
	c_AutoSDK()
	{
		
	}

	void init()
	{
		window = FindWindowA("LWJGL", NULL);
		char wn[500];
		GetWindowTextA(window, wn, 500);

		window_text = wn;
		
		if (window_text.find("Lunar") != std::string::npos)
		{
			global::is_lunar = true;

		}
		else if (window_text.find("Badlion") != std::string::npos)
		{
			if (window_text.find("Client v3") != std::string::npos)
				global::V_3 = true;

			global::is_badlion = true;
		}
		else {
			const std::string& args = GetCommandLineA();

			if (args.find("lunar") != std::string::npos)
			{
				global::is_lunar = true;
			}
			else if (args.find("crystalmod") != std::string::npos)
			{
				//MessageBoxA(0, "crystal", "ok", MB_OK);
				//global::is_badlion = true;
				//global::V_3 = true;
			}
			else if (args.find("LabyMod") != std::string::npos)
			{
				global::is_badlion = true;
				global::V_3 = false;
			}
			else if (args.find("OptiFine") != std::string::npos)
			{
				global::is_badlion = true;
				global::V_3 = false;
			}
			else if (args.find("minecraftforge") == std::string::npos) // Vanilla
			{
				global::is_badlion = true;
				global::V_3 = true;
			}
		}

	}
};