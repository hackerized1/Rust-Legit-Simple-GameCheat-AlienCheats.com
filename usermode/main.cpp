#include "include.h"
#include <vector>
#include "math.h"
#include "main.h"
#include "protect/protectmain.h"
#include "auth.hpp"
#include "driver.h"

using namespace KeyAuth;

std::string namee = "Rust Simple"; // application name. right above the blurred text aka the secret on the licenses tab among other tabs
std::string ownerid = "jHHXUBClti"; // ownerid, found in account settings. click your profile picture on top right of dashboard and then account settings.
std::string secret = "6ea8ddd454bc50a30e29d997de61864e050c3f655d2e74a47d3b7168089ddaad"; // app secret, the blurred text on licenses tab and other tabs
std::string version = "1.2"; // leave alone unless you've changed version on website
std::string url = "https://auth.aliencheats.com/api/1.1/"; // change if you're self-hosting
std::string sslPin = "ssl pin key (optional)"; // don't change unless you intend to pin public certificate key. you can get here in the "Pin SHA256" field https://www.ssllabs.com/ssltest/analyze.html?d=keyauth.win&latest. If you do this you need to be aware of when SSL key expires so you can update it
api KeyAuthApp(namee, ownerid, secret, version, url, sslPin);


using namespace std;

#define BaseGameMode 56617048 // "Signature": "System_Collections_Generic_List_BaseGameMode__c*"
#define BaseGameMode2 56617224 // "Signature": "BaseGameMode_c*"
#define oFovConVarGraphics 56627016 // "Signature": "ConVar_Graphics_c*"
#define ConvarClient 56619288 // "Signature": "ConVar_Client_c*"
#define ConsoleSystem_Index 56726944 //  "Signature": "ConsoleSystem_Index_c*"
#define ConvarAdmin 56614104 // convar_admin_c*
#define OclusionCulling 56625568 // "Signature": "OcclusionCulling_c*"
#define ConVar_Culling_c 56625496 // "Signature": "ConVar_Culling_c*"
#define oPlayerMovement 0x4F8  // public BaseMovement movement;
#define oActiveUID 0x5D8 // private uint clActiveItem;
#define oPlayerInventory 0x6A0 // public PlayerInventory inventory;
#define oContainerBelt 0x28 // public ItemContainer containerBelt
#define oBeltContentList 0x38 // public List<Item> contents   or   public List<int> availableSlots;
#define oHeld 0xA0 // private EntityRef heldEntity
#define oRecoilProperties 0x2E0 // public RecoilProperties recoil;
#define oRecoilYawMin 0x18  // public float recoilYawMin;
#define oRecoilYawMax 0x1C  // public float recoilYawMax;
#define oRecoilPitchMin 0x20 // public float recoilPitchMin;
#define oRecoilPitchMax 0x24 // public float recoilPitchMax;
#define oAimconeCurveScale 0x60 // public float aimconeCurveScale;
#define oStancePenalty 0x324 //private float stancePenalty;
#define oAimCone 0x2F0 //public float aimCone;
#define oHipAimCone 0x2F4 //public float hipAimCone;
#define oAimconePenaltyPerShot 0x2F8 //public float aimconePenaltyPerShot;
#define oAutomatic 0x290 // public bool automatic;
#define capsuleHeight 0x68 // public float capsuleHeight
#define capsuleCenter 0x6C // public float capsuleCenter
#define groundAngle 0xC4 // groundAngle
#define groundAngleNew 0xC8 // groundAngleNew
#define oGravityMultiplier 0x84 // public float gravityMultiplier;

void giris() {
r8:
    if (!driver.init())
    {
    r7:
        if (FindWindowA(skCrypt("UnityWndClass"), NULL))
        {
            printf(skCrypt("[>] Close Rust...\n"));
            Sleep(1000);
            goto r7;
        }
        if (LoadDriver())
        {
            printf(skCrypt("[>] Driver loaded!\n"));
            Sleep(1000);
            //system(skCrypt("cls"));
            goto r8;
        }
    };
    g_pid = get_pid("RustClient.exe");
    driver.attach(g_pid);
    g_assembly = driver.get_process_module(skCrypt("GameAssembly.dll"));
    g_unityplayer = driver.get_process_module(skCrypt("UnityPlayer.dll"));
    if (!g_assembly) { printf(skCrypt("[>] GameAssembly can't found!\n")); return; }
    if (!g_unityplayer) { printf(skCrypt("[>] UnityPlayer can't found\n")); return; }
    //printf("[>] GameAssembly: 0x%p\n[>] Unityplayer: 0x%p\n", g_assembly, g_unityplayer);
}

UINT64 GetHeldItem(UINT64 LocalPlayer)
{
    const auto base_player = LocalPlayer;
    const auto active_item_id = driver.read<UINT64>(base_player + oActiveUID);
    const auto player_inventory = driver.read<UINT64>(base_player + oPlayerInventory);
    const auto inventory_belt = driver.read<UINT64>(player_inventory + oContainerBelt);
    const auto belt_contents_list = driver.read<UINT64>(inventory_belt + oBeltContentList);
    const auto belt_contents_sz = driver.read<uint32_t>(belt_contents_list + 0x14);
    const auto belt_contents = driver.read<UINT64>(belt_contents_list + 0x10);
    for (auto i = 0u; i <= 6; i++)
    {
        if (belt_contents != 0)
        {
            const auto current_item = driver.read<UINT64>(belt_contents + (0x20 + (i * 8)));
            if (!current_item) continue;
            if (active_item_id == driver.read<uint32_t>(current_item + 0x28)) return current_item;
        }
    }
}
std::wstring get_item_name(uintptr_t item)
{
    auto unk = driver.read<UINT64>(item + 0x20);
    unk = driver.read<UINT64>(unk + 0x20);
    return driver.read_wstring(unk + 0x14, 14);
}
bool is_weapon(uintptr_t helditem)
{
    auto ItemName = get_item_name(helditem);
    if (ItemName.find(L"rifle") != std::string::npos) return true;
    if (ItemName.find(L"pistol") != std::string::npos) return true;
    if (ItemName.find(L"bow") != std::string::npos) return true;
    if (ItemName.find(L"lmg") != std::string::npos) return true;
    if (ItemName.find(L"shotgun") != std::string::npos) return true;
    if (ItemName.find(L"smg") != std::string::npos) return true;
    else return false;
}

bool BoxESP;
bool AdminFlag;
bool Spiderman;
bool TimeChanger;
float Time = 12;
bool GraphicFov;
float Fov = 90;
bool NoRecoil;
float NoRecoilINT;
bool NoSpread;
bool MagicBullet;
float MagicBulletINT;
bool Automatic;
bool NoSway;
bool SuperEoka;
bool ThirdPerson;
bool Flyhack = false;
bool Climb = false;
bool InfiniteJump = false;
bool HardcoreModeMap = false;
bool FakeWorkbench = false;

static int FlyHackButon = 0x46;
static int climbbutton = 0x58;

void cheat()
{
    while (true)
    {
        auto main_offsets = driver.read<uintptr_t>(g_assembly + BaseGameMode);
        Local_Player = driver.read_chain(main_offsets, { 0xb8, 0x20, 0x10, 0x28, 0x18, 0x20 });
        auto Movement = driver.read<uintptr_t>(Local_Player + oPlayerMovement);
        auto OcclusionCulling = driver.read<uintptr_t>(g_assembly + OclusionCulling);
        auto CullingDebugOnOff = driver.read<uintptr_t>(OcclusionCulling + 0xB8);
        auto OcclusionCullingEditted = driver.read_chain(OcclusionCulling, { 0xB8, 128, 0x18 });
        auto ConvarCulling = driver.read<uintptr_t>(g_assembly + ConVar_Culling_c);
        auto ConvarCulling2 = driver.read<uintptr_t>(ConvarCulling + 0xB8);
        const auto current_item = GetHeldItem(Local_Player);
        auto base_projectile = driver.read<uintptr_t>(current_item + oHeld);
        auto recoil_prop = driver.read<uintptr_t>(base_projectile + oRecoilProperties);
        auto newRecoilOverride = driver.read<uintptr_t>(recoil_prop + 0x78);
        bool isWeapon = is_weapon(current_item);
        if (HardcoreModeMap) 
        {
            auto HardCoreMap = driver.read<uintptr_t>(g_assembly + BaseGameMode2); //       "Signature": "BaseGameMode_c*"	
            auto HardCoreMapOn = driver.read_chain(HardCoreMap, { 0xb8, 0x8 });
            driver.write<bool>(HardCoreMapOn + 0x17C, true); // public bool ingameMap;
            driver.write<bool>(HardCoreMapOn + 0x17D, true); // public bool compass;
        }
        if (InfiniteJump)
        {
            if (GetAsyncKeyState(VK_SPACE))
            {
                driver.write<Vector3>(Movement + 0xc8, Vector3(0, 10000000, 0));
                driver.write<Vector3>(Movement + 0xc4, Vector3(99999999, 99999999, 99999999));
            }
        }
        if (Climb) 
        {
            if (GetAsyncKeyState(climbbutton))
            {

                driver.write<float>(Movement + capsuleHeight, -100);
                driver.write<float>(Movement + capsuleCenter, -100);
            }
            else
            {
                if (driver.read<float>(Movement + capsuleHeight) != 1.79f) {
                    driver.write<float>(Movement + capsuleHeight, 1.79f);
                    driver.write<float>(Movement + capsuleCenter, 0.899f);
                }
            }
        }
        if (Flyhack)
        {
            if (GetAsyncKeyState(FlyHackButon))
            {

               driver.write<float>(Movement + groundAngle, 0);//groundAngle
               driver.write<float>(Movement + groundAngleNew, 0);//groundAngleNew
               driver.write<float>(Movement + oGravityMultiplier, 0);//gravityMultiplier

            }
            else
            {
                if (driver.read<float>(Movement + oGravityMultiplier) != 2.5f) {
                    driver.write<float>(Movement + oGravityMultiplier, 2.5f);//gravityMultiplier
                }
            }
        }
        if (FakeWorkbench)
        {
            driver.write<uintptr_t>(Local_Player + 0x690, 4194304);
        }
        if (AdminFlag)
        {
            driver.write<uintptr_t>(Local_Player + 0x690, 4);

            auto klass = driver.read<uintptr_t>(g_assembly + ConvarClient);
            auto static_fields = driver.read<uintptr_t>(klass + 0xB8);

            driver.write<float>(static_fields + 0x2C, 1.f);
            driver.write<float>(static_fields + 0x20, 1.f);
            driver.write<bool>(Movement + 0x18, false); // noclipi oto kapatýyor

            for (int x = 0; x < 500; x++)
            {
                auto ConsoleSystemCommand5 = driver.read<uintptr_t>(g_assembly + ConsoleSystem_Index);
                auto ConsoleSystemCommand2 = driver.read<uintptr_t>(ConsoleSystemCommand5 + 0xB8);
                uintptr_t ConsoleSystemCommands = driver.read<uintptr_t>(ConsoleSystem_Index + 0x10);
                if (!ConsoleSystemCommands) return;
                uintptr_t ConsoleSystemCommand = driver.read<uintptr_t>(ConsoleSystemCommands + 0x20 + (x * 0x8));
                if (!ConsoleSystemCommand) continue;

                uintptr_t Name = driver.read<uintptr_t>(ConsoleSystemCommand + 0x10);
                if (!Name) continue;

                wchar_t command_name[36] = { '\0' };
                driver.read_array(Name + 0x14, &command_name, sizeof(command_name));

                if (!wcscmp(command_name, L"debugcamera"))
                {
                    driver.write<bool>(ConsoleSystemCommand + 0x57, 0);
                }
            }
        }
        if (ThirdPerson)
        {
            int flag = driver.read<int>(Local_Player + 0x690);
            driver.write<uintptr_t>(Local_Player + 0x690, 1024);
        }
        if (BoxESP)
        {
            uintptr_t instance = driver.read<uintptr_t>(CullingDebugOnOff + 0x80);
            if (!instance) return;
            uintptr_t debugSettings = driver.read<uintptr_t>(instance + 0x18);
            if (!debugSettings) return;


            driver.write<float>(ConvarCulling2 + 0x0, 99999.f); // culling.entityupdaterate 1000 komutu
            driver.write<uintptr_t>(OcclusionCullingEditted + 0x20, 131072);
            driver.write<uintptr_t>(CullingDebugOnOff + 0x94, 1); // culling.debug 1 komutu
        }
        else
        {
            driver.write<uintptr_t>(CullingDebugOnOff + 0x94, 0);// culling.debug 0 komutu
        }
        if (TimeChanger)
        {
            auto Time1 = driver.read<uintptr_t>(g_assembly + ConvarAdmin);
            auto Time2 = driver.read<uintptr_t>(Time1 + 0xb8);
            driver.write<float>(Time2 + 0x0, Time); // public static float admintime;
        }
        if (Spiderman)
        {
            if (!Movement) return;
            driver.write<float>(Movement + 0xC4, 0.f);
            driver.write<float>(Movement + 0xC8, 0.f);
        }
        if (GraphicFov)
        {
            uint64_t fov = driver.read<UINT64>(g_assembly + oFovConVarGraphics);
            uint64_t instance = driver.read<UINT64>(fov + 0xB8);
            driver.write<float>(instance + 0X18, Fov);
        }
        if (isWeapon) {
            if (NoRecoil) {
                if (newRecoilOverride) {
                    if (driver.read<int>(newRecoilOverride + oRecoilYawMin) != NoRecoilINT) {
                        driver.write<int>(newRecoilOverride + oRecoilYawMin, NoRecoilINT);
                        driver.write<int>(newRecoilOverride + oRecoilYawMax, NoRecoilINT);
                        driver.write<int>(newRecoilOverride + oRecoilPitchMin, NoRecoilINT);
                        driver.write<int>(newRecoilOverride + oRecoilPitchMax, NoRecoilINT);
                    }
                }
                if (!newRecoilOverride) {
                    if (driver.read<int>(recoil_prop + oRecoilYawMin) != NoRecoilINT) {
                        driver.write<int>(recoil_prop + oRecoilYawMin, NoRecoilINT);
                        driver.write<int>(recoil_prop + oRecoilYawMax, NoRecoilINT);
                        driver.write<int>(recoil_prop + oRecoilPitchMin, NoRecoilINT);
                        driver.write<int>(recoil_prop + oRecoilPitchMax, NoRecoilINT);
                    }
                }
            }
            if (MagicBullet)
            {
                auto List = driver.read<uintptr_t>(base_projectile + 0x370); //private List<Projectile> createdProjectiles;
                int size = driver.read<uintptr_t>(List + 0x18);
                List = driver.read<uintptr_t>(List + 0x10);
                for (int i = 0; i < size; ++i)
                {
                    auto Item = driver.read<uintptr_t>(List + 0x20 + (i * 0x8));
                    driver.write<float>(Item + 0x2C, MagicBulletINT);
                }
            }
            if (NoSpread)
            {

                if (newRecoilOverride)
                {
                    if (driver.read<int>(newRecoilOverride + oAimconeCurveScale) != -1) {
                        driver.write<int>(newRecoilOverride + oAimconeCurveScale, -1);
                        driver.write<int>(newRecoilOverride + oStancePenalty, -1);
                        driver.write<int>(newRecoilOverride + oAimCone, -1);
                        driver.write<int>(newRecoilOverride + oHipAimCone, -1);
                        driver.write<int>(newRecoilOverride + oAimconePenaltyPerShot, -1);
                    }
                }
                if (!newRecoilOverride)
                {
                    if (driver.read<int>(recoil_prop + oAimconeCurveScale) != -1) {
                        driver.write<int>(recoil_prop + oAimconeCurveScale, -1);
                        driver.write<int>(recoil_prop + oStancePenalty, -1);
                        driver.write<int>(recoil_prop + oAimCone, -1);
                        driver.write<int>(recoil_prop + oHipAimCone, -1);
                        driver.write<int>(recoil_prop + oAimconePenaltyPerShot, -1);
                    }
                }
            }
            if (Automatic)
            {
                if (driver.read<bool>(base_projectile + oAutomatic) != Automatic) {
                    driver.write<bool>(base_projectile + oAutomatic, Automatic);
                }
            }
            if (NoSway)
            {
                driver.write<float>(base_projectile + 0x2D8, 0.f);// public float aimSway;
                driver.write<float>(base_projectile + 0x2DC, 0.f);// public float aimSwaySpeed;
            }
            if (SuperEoka)
            {
                driver.write<float>(base_projectile + 0x378, 100.f); // successFraction
                driver.write<bool>(base_projectile + 0x388, true); // didSparkThisFrame
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}
int tab = 1;
bool GirisYapildi = false;
int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, LOADER_BRAND, NULL };
    RegisterClassEx(&wc);
    main_hwnd = CreateWindow(wc.lpszClassName, LOADER_BRAND, WS_POPUP, 0, 0, 5, 5, NULL, NULL, wc.hInstance, NULL);
    if (!CreateDeviceD3D(main_hwnd)) {
        CleanupDeviceD3D();
        UnregisterClass(wc.lpszClassName, wc.hInstance);
        return 1;
    }
    ShowWindow(main_hwnd, SW_HIDE);
    UpdateWindow(main_hwnd);

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.Fonts->AddFontFromMemoryCompressedBase85TTF(RobotoLight_compressed_data_base85, 13.5F);
    io.IniFilename = nullptr;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {}
    ImFontConfig font_config;
    font_config.OversampleH = 1; //or 2 is the same
    font_config.OversampleV = 1;
    font_config.PixelSnapH = 1;
    static const ImWchar ranges[] =
    {
        0x0020, 0x00FF, // Basic Latin + Latin Supplement
        0x0400, 0x044F, // Cyrillic
        0,
    };
    //ImFont* name = io.Fonts->AddFontFromFileTTF("../../misc/fonts/Museo Sans Cyrl 700.ttf", 19.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    ImFont* icons = io.Fonts->AddFontFromMemoryTTF((void*)icon, sizeof(icon), 38, &font_config, ranges);
    //ImFont* weapon_icons = io.Fonts->AddFontFromFileTTF("../../misc/fonts/icomoon.ttf", 38.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //ImFont* font = io.Fonts->AddFontFromFileTTF("../../misc/fonts/Museo Sans Cyrl 700.ttf", 13.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    ImFont* font = io.Fonts->AddFontFromMemoryCompressedTTF(Pixel_compressed_data, Pixel_compressed_size, 13.f, NULL, io.Fonts->GetGlyphRangesJapanese());
    ImFont* fonth = io.Fonts->AddFontFromMemoryCompressedTTF(Pixel_compressed_data, Pixel_compressed_size, 15.f, NULL, io.Fonts->GetGlyphRangesJapanese());
    ImGui_ImplWin32_Init(main_hwnd);
    ImGui_ImplDX9_Init(g_pd3dDevice);
    LPDIRECT3DTEXTURE9 lg = nullptr;
    if (lg == nullptr) {
        D3DXCreateTextureFromFileInMemory(g_pd3dDevice, &logo, sizeof(logo), &lg);
    }
    DWORD window_flags = ImGuiWindowFlags_::ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
    RECT screen_rect;
    GetWindowRect(GetDesktopWindow(), &screen_rect);
    auto x = float(screen_rect.right - WINDOW_WIDTH) / 2.f;
    auto y = float(screen_rect.bottom - WINDOW_HEIGHT) / 2.f;
    MSG msg;
    ZeroMemory(&msg, sizeof(msg));
    int MenuSayfasi = 0;
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            continue;
        }
        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        {
            auto s = ImVec2{}, p = ImVec2{}, gs = ImVec2{ 620, 485 };
            ImGui::SetNextWindowPos(ImVec2(x, y), ImGuiCond_Once);
            ImGui::SetNextWindowSize(ImVec2(gs));
            ImGui::SetNextWindowBgAlpha(1.0f);
            ImGui::Begin(LOADER_BRAND, &loader_active, window_flags);
            {
                ImVec2 curPos = ImGui::GetCursorPos();
                ImVec2 curWindowPos = ImGui::GetWindowPos();
                curPos.x += curWindowPos.x;
                curPos.y += curWindowPos.y;
                ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(curPos.x, 0), ImVec2(curPos.x + 485, curPos.y - 7), ImColor(255, 255, 0)); // kýrmýzý bar	//static float flRainbow;
                float flSpeed = 0.0010f;
                static float flRainbow;
                flRainbow += flSpeed;
                ImColor cRainbow;
                if (flRainbow > 1.f) flRainbow = 0.f;
                for (int i = 0; i < 485; i++)
                {
                    float hue = (1.f / (float)485) * i;
                    hue -= flRainbow;
                    if (hue < 0.f) hue += 1.f;

                    cRainbow = ImColor::HSV(hue, 1.f, 1.f);
                    //ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(curPos.x + i, curPos.y - 5), ImVec2(curPos.x + 485, curPos.y - 7), cRainbow);
                }
                ImGui::SetCursorPosX(121);
                s = ImVec2(ImGui::GetWindowSize().x - ImGui::GetStyle().WindowPadding.x * 2, ImGui::GetWindowSize().y - ImGui::GetStyle().WindowPadding.y * 2); p = ImVec2(ImGui::GetWindowPos().x + ImGui::GetStyle().WindowPadding.x, ImGui::GetWindowPos().y + ImGui::GetStyle().WindowPadding.y); auto draw = ImGui::GetWindowDrawList();
                draw->AddRectFilled(p, ImVec2(p.x + s.x, p.y + s.y), ImColor(15, 17, 19), 5);
                draw->AddRectFilled(ImVec2(p.x, p.y + 40), ImVec2(p.x + s.x, p.y + s.y), ImColor(18, 20, 21), 5, ImDrawCornerFlags_Bot);
                //draw->AddImage(lg, ImVec2(p.x - 60, p.y - 30), ImVec2(p.x + 110, p.y + 70), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 102));
                ImGui::PushFont(icons);
                auto logo_size = ImGui::CalcTextSize(skCrypt("A"));
                draw->AddText(ImVec2(p.x + 28 - logo_size.x / 2, p.y + 29 - (logo_size.y / 2) + 4), cRainbow, skCrypt("A"));
                ImGui::PopFont();
                ImGui::PushFont(fonth);
                auto logo_size2 = ImGui::CalcTextSize(skCrypt("A"));
                draw->AddText(ImVec2(p.x + 42 - logo_size2.x / 2, p.y + 29 - logo_size2.y), cRainbow, skCrypt("AlienCheats"));
                ImGui::PopFont();
                ImGui::PushFont(font);
                draw->AddText(ImVec2(p.x + 42 - logo_size2.x / 2, p.y + 40 - logo_size2.y), cRainbow, skCrypt("(Simple)"));
                ImGui::PopFont();
                ImGui::PushFont(font);
                ImGui::BeginGroup();
                ImGui::SameLine(120);
                if (ImGui::tab(skCrypt("Main"), tab == 1))tab = 1; ImGui::SameLine();
                if (ImGui::tab(skCrypt("Misc"), tab == 2))tab = 2; ImGui::SameLine();
                ImGui::EndGroup();
                ImGui::PopFont();

                if (tab == 1)
                {
                    ImGui::PushFont(font);
                    {//left upper
                        ImGui::SetCursorPosY(50);
                        ImGui::BeginGroup();
                        ImGui::SetCursorPosX(15);
                        ImGui::MenuChild(skCrypt("General"), ImVec2(300, 205), false);
                        {
                            ImGui::Checkbox(skCrypt("BoxESP"), &BoxESP);
                            ImGui::Checkbox(skCrypt("AdminFlag"), &AdminFlag);
                            ImGui::Checkbox(skCrypt("ThirdPerson"), &ThirdPerson);
                        }                   
                        ImGui::EndChild();
                        ImGui::EndGroup();
                    }
                    {//left bottom  
                        ImGui::SetCursorPosY(255);
                        ImGui::BeginGroup();
                        ImGui::SetCursorPosX(15);
                        ImGui::MenuChild(skCrypt("Settings"), ImVec2(300, 210), false);
                        {
                            ImGui::Checkbox(skCrypt("TimeChanger"), &TimeChanger);
                            ImGui::Text(skCrypt(""));
                            ImGui::SliderFloat(skCrypt("TimeChanger"), &Time, 0.0f, 24.f);
                            ImGui::Checkbox(skCrypt("GraphicFov"), &GraphicFov);
                            ImGui::Text(skCrypt(""));
                            ImGui::SliderFloat(skCrypt("GraphicFov"), &Fov, 90.0f, 160.f);
                        }
                        ImGui::EndChild();
                        ImGui::EndGroup();
                    }
                    {//right
                        ImGui::SetCursorPosY(50);
                        ImGui::BeginGroup();
                        ImGui::SetCursorPosX(320);
                        ImGui::MenuChild(skCrypt("Weapon"), ImVec2(285, 415), false);
                        {
                            ImGui::Checkbox(skCrypt("NoRecoil"), &NoRecoil);
                            ImGui::SliderFloat(skCrypt("NoRecoil"), &NoRecoilINT, 0.0f, 20.f);
                            ImGui::Checkbox(skCrypt("NoSpread"), &NoSpread);
                            ImGui::Checkbox(skCrypt("ThickBullet"), &MagicBullet);
                            ImGui::SliderFloat(skCrypt("ThickBullet"), &MagicBulletINT, 0.0f, 10.f);
                            ImGui::Checkbox(skCrypt("Automatic"), &Automatic);
                            ImGui::Checkbox(skCrypt("NoSway"), &NoSway);
                            ImGui::Checkbox(skCrypt("SuperEoka"), &SuperEoka);
                        }
                        ImGui::EndChild();
                        ImGui::EndGroup();
                        ImGui::PopFont();
                    }
                }
                if (tab == 2)
                {
                    ImGui::PushFont(font);
                    {//left upper
                        ImGui::SetCursorPosY(50);
                        ImGui::BeginGroup();
                        ImGui::SetCursorPosX(15);
                        ImGui::MenuChild(skCrypt("Misc"), ImVec2(285, 415), false);
                        {
                            ImGui::Checkbox(skCrypt("Spiderman"), &Spiderman);
                            ImGui::Checkbox(skCrypt("Flyhack"), &Flyhack);
                            ImGui::KeyBind(skCrypt("FlyHack Key"), FlyHackButon, ImVec2(50, 0));
                            ImGui::Checkbox(skCrypt("Climb"), &Climb);
                            ImGui::KeyBind(skCrypt("Climb Key"), climbbutton, ImVec2(50, 0));
                            ImGui::Checkbox(skCrypt("InfiniteJump"), &InfiniteJump);
                            ImGui::Checkbox(skCrypt("HardcoreModeMap"), &HardcoreModeMap);
                            ImGui::Checkbox(skCrypt("FakeWorkbench"), &FakeWorkbench);
                        }
                        ImGui::EndChild();
                        ImGui::EndGroup();
                    }
                    {//right
                        ImGui::SetCursorPosY(50);
                        ImGui::BeginGroup();
                        ImGui::SetCursorPosX(320);
                        ImGui::MenuChild(skCrypt("CONFIG"), ImVec2(285, 415), false);
                        {

                        }
                        ImGui::EndChild();
                        ImGui::EndGroup();
                    }
                    ImGui::PopFont();
                }
            }
            ImGui::End();
        }
        ImGui::EndFrame();

        g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
        if (g_pd3dDevice->BeginScene() >= 0)
        {
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
            g_pd3dDevice->EndScene();
        }


        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }

        HRESULT result = g_pd3dDevice->Present(NULL, NULL, NULL, NULL);


        if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET) {
            ResetDevice();
        }
        if (!loader_active) {
            msg.message = WM_QUIT;
        }
    }

    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    DestroyWindow(main_hwnd);
    UnregisterClass(wc.lpszClassName, wc.hInstance);

    return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
        {
            g_d3dpp.BackBufferWidth = LOWORD(lParam);
            g_d3dpp.BackBufferHeight = HIWORD(lParam);
            ResetDevice();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU)
            return 0;
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

static std::string randomclassname = "Rust-Cheat";

int main()
{
    SetConsoleTitle(randomclassname.c_str());
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(handle, &info);
    COORD new_size =
    {
        info.srWindow.Right - info.srWindow.Left + 1,
        info.srWindow.Bottom - info.srWindow.Top + 1
    };
    SetConsoleScreenBufferSize(handle, new_size);

    HWND hwnd = GetConsoleWindow();
    if (hwnd != NULL) { MoveWindow(hwnd, 200, 100, 320, 550, TRUE); }
    SetLayeredWindowAttributes(hwnd, 1000, 1000, LWA_ALPHA);

    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO     cursorInfo;

    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(out, &cursorInfo);

    SetWindowLong(hwnd, GWL_STYLE, GetWindowLong(hwnd, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);

    SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
    //SetConsole();

    SetConsoleTitleA(randomclassname.c_str());
    mainprotect();


    KeyAuthApp.init();

    std::string DosyaKonumu = (_xor_("C:\\Windows\\System\\RustSimple.txt"));

EnBasaDon:
    std::string License;
    FILE* Dosya;
    if (Dosya = fopen((DosyaKonumu.c_str()), skCrypt("r"))) {
        std::ifstream DosyaOku(DosyaKonumu.c_str());
        std::string Anahtar;
        std::getline(DosyaOku, Anahtar);
        DosyaOku.close();
        fclose(Dosya);
        License = Anahtar;
        goto LisansaGit;
    }
    else
    {
        SetConsoleTitleA(skCrypt("  "));

        system(skCrypt("cls"));
        std::cout << skCrypt("\n\n  [+] Connecting..");
        std::cout << skCrypt("\n\n  [+] Enter License: ");
        std::cin >> License;
    LisansaGit:
        std::ofstream key; key.open(DosyaKonumu.c_str());
        key << License;
        key.close();
        KeyAuthApp.license(License);
        if (!KeyAuthApp.data.success)
        {
            std::cout << _xor_("\n Status: ").c_str() + KeyAuthApp.data.message;
            Sleep(1500);
            remove(DosyaKonumu.c_str());
            goto EnBasaDon;
            //exit(0);
        }
        system(skCrypt("cls"));
        Sleep(300);
        std::cout << skCrypt("\n\n  [+] License Activated.") << std::endl;;
    }

    ::SetWindowPos(GetConsoleWindow(), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_HIDEWINDOW);
	giris();
	Sleep(1);
	std::thread(cheat).detach();
    WinMain(0, 0, 0, 0);
}