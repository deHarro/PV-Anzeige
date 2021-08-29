#include "Types.h"

std::vector<StringConfig> StringConfig::fromMsgPack(const QVariant& variant)
{
    const auto strings = variant.toList();
    std::vector<StringConfig> configs;
    configs.reserve(strings.size());
    for (const auto& string : strings) {
        StringConfig config;
        config.name = string.toMap().value(toIntString(InverterProperty::StringName)).toString();
        //config.inverterSerial =
        //config.azimuth = 180.0f;
        //config.elevation = 30.0f;
        config.powerPeak = string.toMap().value(toIntString(InverterProperty::StringPowerMax)).toFloat();

        configs.push_back(config);
    }

    return configs;
}

bool InverterConfig::isValid() const
{
    return serial != 0;
}

QString toIntString(InverterProperty prop) {
    return QString::number(static_cast<int>(prop));
}

StringData::StringData(QObject* parent)
    : QObject(parent)
{
}

StringData::~StringData()
{
}
