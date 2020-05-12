/*
* Copyright 2002, Log4cpp Project. All rights reserved.
*
* See the COPYING file for the terms of usage and distribution.
*/


#include "appendersfactory.h"
#include <stdexcept>

static AppendersFactory* appenders_factory_ = 0;

std::auto_ptr<Appender> create_file_appender(const FactoryParams&);
//std::auto_ptr<Appender> create_remote_syslog_appender(const FactoryParams&);
//std::auto_ptr<Appender> create_http_appender(const FactoryParams&);
std::auto_ptr<Appender> create_roll_file_appender(const FactoryParams& params);
std::auto_ptr<Appender> create_console_appender(const FactoryParams& params);

AppendersFactory& AppendersFactory::getInstance()
{
	if (!appenders_factory_)
	{
		std::auto_ptr<AppendersFactory> af(new AppendersFactory);

		af->registerCreator("file", &create_file_appender);
		//af->registerCreator("remote syslog", &create_remote_syslog_appender);
		//af->registerCreator("http", &create_http_appender);
		af->registerCreator("rolling file", &create_roll_file_appender);
		af->registerCreator("console", &create_console_appender);
		appenders_factory_ = af.release();
	}

	return *appenders_factory_;
}

void AppendersFactory::registerCreator(const std::string& class_name, create_function_t create_function)
{
	const_iterator i = creators_.find(class_name);
	if (i != creators_.end())
		throw std::invalid_argument("Appender creator for type name '" + class_name + "' already registered");

	creators_[class_name] = create_function;
}

std::auto_ptr<Appender> AppendersFactory::create(const std::string& class_name, const params_t& params)
{
	const_iterator i = creators_.find(class_name);
	if (i == creators_.end())
		throw std::invalid_argument("There is no appender with type name '" + class_name + "'");

	return (*i->second)(params);
}

bool AppendersFactory::registered(const std::string& class_name) const
{
	return creators_.end() != creators_.find(class_name);
}

