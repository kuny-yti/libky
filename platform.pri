
win32 {
    winrt|wince{
         Platform = WinRT_CE
    }
    else{
         Platform = Win32
    }
}
#else:unix{
#    Platform = Unix
#}
else:mac{
    macx{
         Platform = Mac
    }
    else:ios{
         Platform = IOS
    }
    else{
         Platform = Mac
    }
}
else:android{
    Platform = Android
}
else{ #posix
    Platform = Linux
}

debug{
BuildLevel = debug
}
else:release{
BuildLevel = release
}
