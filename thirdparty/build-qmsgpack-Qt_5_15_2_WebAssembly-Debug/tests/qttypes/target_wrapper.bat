@echo off
SetLocal EnableDelayedExpansion
(set PATH=U:\Qt\5.15.2\wasm_32\bin;!PATH!)
if defined QT_PLUGIN_PATH (
    set QT_PLUGIN_PATH=U:\Qt\5.15.2\wasm_32\plugins;!QT_PLUGIN_PATH!
) else (
    set QT_PLUGIN_PATH=U:\Qt\5.15.2\wasm_32\plugins
)
%*
EndLocal
