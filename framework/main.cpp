#include "lib/core/XLatte.h"
#include "lib/net/XDownloader.h"
#include "lib/net/updater/XUpdater.h"
#include "lib/core/XApplication.h"
#include "lib/base/global.h"


#include <iostream>
#include <memory>
#include <thread>

int main()
{
	XApplication::getInstance()->setAnimationInterval(1.0 / 30);

	
	//downloader->downloadAsync("http://www.my_work.com/php/test/cocos2dx-update-temp-package.zip", "./cocos2dx-update-temp-package.zip", "000001");
	//downloader->downloadAsync("http://shezzer.sinaapp.com/downloadTest/cocos2dx-update-temp-package.zip", "./cocos2dx-update-temp-package.zip", "000001");
	//开个背后线程做事
	std::thread t_back = std::thread(&XApplication::run, (XApplication::getInstance()));
	t_back.detach();

	std::string inputStr = "";
	while (std::cin>>inputStr)
	{
		if (inputStr.compare("download")==0)
		{
			std::cout << "download start..." << std::endl;
			std::shared_ptr<XDownloader> downloader(new XDownloader());
			downloader->downloadAsync("http://shezzer.sinaapp.com/downloadTest/cocos2dx-update-temp-package.zip", "./cocos2dx-update-temp-package.zip", "000001");
		}
		else if (inputStr.compare("check") == 0)
		{
			std::cout << "check version..." << std::endl;
			std::shared_ptr<XUpdater> updater(new XUpdater("http://shezzer.sinaapp.com/downloadTest/cocos2dx-update-temp-package.zip", "./cocos2dx-update-temp-package.zip", "000001"));
			updater->checkVersion();
		}
		else
		{
			std::cout << "unknown command" << std::endl;
		}
		//std::cout << inputStr << std::endl;
	}

	//XApplication::getInstance()->run();
	system("pause");
	return 0;
}