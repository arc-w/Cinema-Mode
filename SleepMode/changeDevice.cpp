#include "header.h"

void enumDevices() // enumerates available audio devices
{
    int nSoundCardCount = waveOutGetNumDevs();

    for (int i = 0; i < nSoundCardCount; i++)
    {
        WAVEOUTCAPS woc;
        waveOutGetDevCaps(i, &woc, sizeof(woc));

        std::cout << woc.szPname << std::endl;
    }
}

HRESULT SetDefaultAudioPlaybackDevice(LPCWSTR devID) // raw functions that sets Default Audio Playback Device
{
    IPolicyConfigVista* pPolicyConfig;
    ERole reserved = eConsole;

    HRESULT hr = CoCreateInstance(__uuidof(CPolicyConfigVistaClient),
        NULL, CLSCTX_ALL, __uuidof(IPolicyConfigVista), (LPVOID*)&pPolicyConfig);
    if (SUCCEEDED(hr))
    {
        hr = pPolicyConfig->SetDefaultEndpoint(devID, reserved);
        pPolicyConfig->Release();
    }
    return hr;
}

//Set the default audio playback device 

void InitDefaultAudioDevice(CString device)
{
    HRESULT hr = CoInitialize(NULL);
    if (SUCCEEDED(hr))
    {
        IMMDeviceEnumerator* pEnum = NULL;
        // Create a multimedia device enumerator.
        hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL,
            CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&pEnum);
        if (SUCCEEDED(hr))
        {
            //Determine if it is the default audio device
            bool bExit = false;
            IMMDevice* pDefDevice = NULL;
            hr = pEnum->GetDefaultAudioEndpoint(eRender, eMultimedia, &pDefDevice);
            if (SUCCEEDED(hr))
            {
                IPropertyStore* pStore;
                hr = pDefDevice->OpenPropertyStore(STGM_READ, &pStore);
                if (SUCCEEDED(hr))
                {
                    PROPVARIANT friendlyName;
                    PropVariantInit(&friendlyName);
                    hr = pStore->GetValue(PKEY_Device_FriendlyName, &friendlyName);
                    if (SUCCEEDED(hr))
                    {
                        CString strTmp = friendlyName.pwszVal;
                        if (strTmp.Find(device) != -1)
                        {
                            bExit = true;
                        }
                        PropVariantClear(&friendlyName);
                    }
                    pStore->Release();
                }
                pDefDevice->Release();
            }
            if (bExit)
            {
                pEnum->Release();
                return;
            }

            IMMDeviceCollection* pDevices;
            // Enumerate the output devices.
            hr = pEnum->EnumAudioEndpoints(eRender, DEVICE_STATE_ACTIVE, &pDevices);
            if (SUCCEEDED(hr))
            {
                UINT count;
                pDevices->GetCount(&count);
                if (SUCCEEDED(hr))
                {
                    for (int i = 0; i < count; i++)
                    {
                        bool bFind = false;
                        IMMDevice* pDevice;
                        hr = pDevices->Item(i, &pDevice);
                        if (SUCCEEDED(hr))
                        {
                            LPWSTR wstrID = NULL;
                            hr = pDevice->GetId(&wstrID);
                            if (SUCCEEDED(hr))
                            {
                                IPropertyStore* pStore;
                                hr = pDevice->OpenPropertyStore(STGM_READ, &pStore);
                                if (SUCCEEDED(hr))
                                {
                                    PROPVARIANT friendlyName;
                                    PropVariantInit(&friendlyName);
                                    hr = pStore->GetValue(PKEY_Device_FriendlyName, &friendlyName);
                                    if (SUCCEEDED(hr))
                                    {
                                        // if no options, print the device
                                        // otherwise, find the selected device and set it to be default
                                        CString strTmp = friendlyName.pwszVal;
                                        if (strTmp.Find(device) != -1)
                                        {
                                            SetDefaultAudioPlaybackDevice(wstrID);
                                            bFind = true;
                                        }
                                        PropVariantClear(&friendlyName);
                                    }
                                    pStore->Release();
                                }
                            }
                            pDevice->Release();
                        }

                        if (bFind)
                        {
                            break;
                        }
                    }
                }
                pDevices->Release();
            }
            pEnum->Release();
        }
    }
    CoUninitialize();
}

std::vector<CString> CoutDefaultAudioDevices() // prints all available audio devices, returns vector of those devices in CString
{
    std::vector<CString> devicesList;
    HRESULT hr = CoInitialize(NULL);
    if (SUCCEEDED(hr))
    {
        IMMDeviceEnumerator* pEnum = NULL;
        // Create a multimedia device enumerator.
        hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL,
            CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&pEnum);
        if (SUCCEEDED(hr))
        {
            //Determine if it is the default audio device
            bool bExit = false;
            IMMDevice* pDefDevice = NULL;
            hr = pEnum->GetDefaultAudioEndpoint(eRender, eMultimedia, &pDefDevice);

            IMMDeviceCollection* pDevices;
            // Enumerate the output devices.
            hr = pEnum->EnumAudioEndpoints(eRender, DEVICE_STATE_ACTIVE, &pDevices);
            if (SUCCEEDED(hr))
            {
                UINT count;
                pDevices->GetCount(&count);
                if (SUCCEEDED(hr))
                {
                    for (int i = 0; i < count; i++)
                    {
                        bool bFind = false;
                        IMMDevice* pDevice;
                        hr = pDevices->Item(i, &pDevice);
                        if (SUCCEEDED(hr))
                        {
                            LPWSTR wstrID = NULL;
                            hr = pDevice->GetId(&wstrID);
                            if (SUCCEEDED(hr))
                            {
                                IPropertyStore* pStore;
                                hr = pDevice->OpenPropertyStore(STGM_READ, &pStore);
                                if (SUCCEEDED(hr))
                                {
                                    PROPVARIANT friendlyName;
                                    PropVariantInit(&friendlyName);
                                    hr = pStore->GetValue(PKEY_Device_FriendlyName, &friendlyName);
                                    if (SUCCEEDED(hr))
                                    {
                                        CString strTmp = friendlyName.pwszVal;
                                        std::cout << i << ": ";
                                        std::wcout << (LPCTSTR)strTmp << std::endl;
                                        devicesList.push_back(strTmp);
                                        PropVariantClear(&friendlyName);
                                    }
                                    pStore->Release();
                                }
                            }
                            pDevice->Release();
                        }

                        if (bFind)
                        {
                            break;
                        }
                    }
                }
                pDevices->Release();
            }
            pEnum->Release();
        }
    }
    CoUninitialize();
    return devicesList;
}