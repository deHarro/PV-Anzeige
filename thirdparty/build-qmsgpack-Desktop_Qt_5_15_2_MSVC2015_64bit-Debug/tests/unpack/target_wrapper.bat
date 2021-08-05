@echo off
SetLocal EnableDelayedExpansion
(set PATH=U:\Qt\5.15.2\msvc2015_64\bin;!PATH!)
if defined QT_PLUGIN_PATH (
    set QT_PLUGIN_PATH=U:\Qt\5.15.2\msvc2015_64\plugins;!QT_PLUGIN_PATH!
) else (
    set QT_PLUGIN_PATH=U:\Qt\5.15.2\msvc2015_64\plugins
)
%*
EndLocal
