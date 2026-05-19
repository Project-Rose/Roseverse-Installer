#include "eg2/libs/typedefs.h"
#include "eg2/tex/textures.hh"
#include "eg2/tex/font.hh"
#include "eg2/tex/color.hh"
#include "eg2/tex/raw.hh"
#include "eg2/gx2/textures.hh"
#include "eg2/render/screen.hh"
#include "eg2/controller/drc.hh"
#include "inkay/render/gui.hh"
#include "inkay/repos/core.hh"
#include "inkay/repos/versions.hh"
#include "inkay/repos/download.hh"
#include "inkay/fs/dirs.hh"
#include <memory>

namespace Inkay {
    namespace GUI {
        static std::unique_ptr<Tex::LatteFont> OSCAFESTDFont;
        static std::unique_ptr<Tex::LatteColor> ColorChangingBars;
        static std::unique_ptr<Tex::LatteColor> ButtonsHolder;
        static std::unique_ptr<Tex::LatteTex> ProjectRoseLogo;
        static std::unique_ptr<Tex::LatteTex> PretendoLogo;
        static std::unique_ptr<Tex::LatteTex> PortalBG;

        static bool DidAlignFontChange = false;
        static bool AlreadyAppliedGUIChange = false;

        void Init(void) {
            OSCAFESTDFont = std::make_unique<Tex::LatteFont>(FONT_CAFESTD, 24.0f);
            ColorChangingBars = std::make_unique<Tex::LatteColor>(Colors::Default);
            ButtonsHolder = std::make_unique<Tex::LatteColor>(Colors::Holder);
            ProjectRoseLogo = std::make_unique<Tex::LatteTex>("fs:/vol/content/rogo/bara.png");
            PretendoLogo = std::make_unique<Tex::LatteTex>("fs:/vol/content/rogo/pretendo.png");
            PortalBG = std::make_unique<Tex::LatteTex>("fs:/vol/content/bakkuguraundo/bara.png");
        }

        void Render(void) {
            if (PortalBG) PortalBG->RenderBackgroundRepeatScaled(RENDERRATIO_ASPECT_1920x1080);

            if (ColorChangingBars) {
                ColorChangingBars->DrawSolidCenteredScaled(RENDERRATIO_ASPECT_854x480, 1000.0f, 55.0f, 0.0f, -214.0f);
                ColorChangingBars->DrawSolidCenteredScaled(RENDERRATIO_ASPECT_854x480, 1000.0f, 55.0f, 0.0f, 214.0f);
            }

            switch (Inkay::Download::State) {
                case DOWNLOADSTATE_SELECT: {
                    if (ButtonsHolder) {
                        ButtonsHolder->DrawSolidCenteredScaled(RENDERRATIO_ASPECT_854x480, 400.0f, 85.0f, 0.0f, 10.0f);
                        ButtonsHolder->DrawSolidCenteredScaled(RENDERRATIO_ASPECT_854x480, 400.0f, 85.0f, 0.0f, 100.0f);
                    }

                    if (ProjectRoseLogo) ProjectRoseLogo->RenderTexCenteredScaled(RENDERRATIO_ASPECT_854x480, 45.0f, 50.0f, -95.0f, 100.0f);
                    if (PretendoLogo) PretendoLogo->RenderTexCenteredScaled(RENDERRATIO_ASPECT_854x480, 50.0f, 50.0f, -90.0f, 10.0f);

                    if (OSCAFESTDFont) {
                        OSCAFESTDFont->SetColor(Colors::Black);
                        OSCAFESTDFont->RenderTextCenteredScaled(
                            0.0f, -130.0f,
                            RENDERRATIO_ASPECT_854x480, 370.0f, 75.0f,
                            "Select a Inkay version to install"
                        );

                        OSCAFESTDFont->RenderTextCenteredScaled(
                            0.0f, -90.0f,
                            RENDERRATIO_ASPECT_854x480, 420.0f, 100.0f,
                            "Inkay (Roseverse) keeps all other Pretendo functionality intact"
                        );

                        OSCAFESTDFont->SetColor(Colors::Juxt);
                        OSCAFESTDFont->RenderTextCenteredScaled(
                            15.0f, 5.0f,
                            RENDERRATIO_ASPECT_854x480, 170.0f, 100.0f,
                            "Juxtaposition"
                        );

                        OSCAFESTDFont->SetColor(Colors::Default);
                        OSCAFESTDFont->RenderTextCenteredScaled(
                            15.0f, 90.0f,
                            RENDERRATIO_ASPECT_854x480, 160.0f, 100.0f,
                            "Roseverse"
                        );

                        OSCAFESTDFont->SetColor(Colors::Browser);
                        OSCAFESTDFont->RenderTextScaled(
                            RENDERRATIO_ASPECT_854x480, 150.0f, 70.0f,
                            "Help/More Info",
                            690.0f, 435.0f
                        );
                    }

                    break;
                }

                case DOWNLOADSTATE_SELECT_ENV: {
                    usize envCount = Inkay::Dirs::gEnvironments.size();

                    if (envCount > 1) {
                        if (OSCAFESTDFont && ButtonsHolder) {
                            ButtonsHolder->DrawSolidCenteredScaled(
                                RENDERRATIO_ASPECT_854x480, 500.0f, 180.0f,
                                0.0f, 0.0f
                            );

                            OSCAFESTDFont->SetColor(Colors::Black);
                            OSCAFESTDFont->RenderTextCenteredScaled(
                                0.0f, -140.0f,
                                RENDERRATIO_ASPECT_854x480, 450.0f, 70.0f,
                                "Select installation environment"
                            );

                            OSCAFESTDFont->RenderTextCenteredScaled(
                                0.0f, 205.0f,
                                RENDERRATIO_ASPECT_854x480, 150.0f, 60.0f,
                                "A - Confirm"
                            );

                            OSCAFESTDFont->RenderTextCenteredScaled(
                                0.0f, -80.0f,
                                RENDERRATIO_ASPECT_854x480, 90.0f, 50.0f,
                                "^"
                            );

                            usize selected = Inkay::Dirs::gSelectedEnv;

                            OSCAFESTDFont->RenderTextCenteredScaled(
                                0.0f, -20.0f,
                                RENDERRATIO_ASPECT_854x480, 300.0f, 50.0f,
                                Inkay::Dirs::gEnvironments[selected].c_str()
                            );

                            OSCAFESTDFont->RenderTextCenteredScaled(
                                0.0f, 50.0f,
                                RENDERRATIO_ASPECT_854x480, 60.0f, 40.0f,
                                "v"
                            );

                            char counter[64];
                            snprintf(counter, sizeof(counter), "%d / %d", (int)(selected + 1), (int)envCount);

                            OSCAFESTDFont->RenderTextCenteredScaled(
                                0.0f, 130.0f,
                                RENDERRATIO_ASPECT_854x480, 80.0f, 40.0f,
                                counter
                            );

                            OSCAFESTDFont->SetColor(Colors::Browser);
                            OSCAFESTDFont->RenderTextScaled(
                                RENDERRATIO_ASPECT_854x480, 150.0f, 70.0f,
                                "Help/More Info",
                                690.0f, 435.0f
                            );
                        }
                    }

                    break;
                }

                case DOWNLOADSTATE_DOWNLOADING: {
                    if (OSCAFESTDFont) {
                        OSCAFESTDFont->SetColor(Colors::Black);
                        OSCAFESTDFont->RenderTextCenteredScaled(
                            0.0f, 0.0f,
                            RENDERRATIO_ASPECT_854x480, 350.0f, 75.0f,
                            "Downloading"
                        );
                    }

                    break;
                }

                case DOWNLOADSTATE_ERROR: {
                    if (OSCAFESTDFont) {
                        OSCAFESTDFont->SetColor(Colors::Black);

                        if (Inkay::Download::HasError.load(std::memory_order_acquire)) {
                            OSCAFESTDFont->RenderTextCenteredScaled(
                                0.0f, 0.0f,
                                RENDERRATIO_ASPECT_854x480, 500.0f, 90.0f,
                                Inkay::Download::LastError.c_str()
                            );
                        } else {
                            OSCAFESTDFont->RenderTextCenteredScaled(
                                0.0f, 0.0f,
                                RENDERRATIO_ASPECT_854x480, 500.0f, 90.0f,
                                "Unknown error"
                            );
                        }
                    }

                    break;
                }

                case DOWNLOADSTATE_FINISHED: {
                    if (OSCAFESTDFont) {
                        OSCAFESTDFont->SetColor(Colors::Black);
                        OSCAFESTDFont->RenderTextCenteredScaled(
                            0.0f, 0.0f,
                            RENDERRATIO_ASPECT_854x480, 350.0f, 75.0f,
                            "Installed Inkay!\nPress B to Restart"
                        );
                    }

                    break;
                }
            }
        }

        void Update(void) {
            Inkay::Download::UpdateDownloads();
            DRC::Update();

            if (OSCAFESTDFont && !DidAlignFontChange) {
                OSCAFESTDFont->SwitchRenderType(FONT_ALIGN_CENTER);
                DidAlignFontChange = true;
            }

            if (!AlreadyAppliedGUIChange) {
                u32 color = Colors::Default;

                switch (Inkay::Versions::FileState) {
                    case Inkay::Versions::LocalFileState::ValidJuxt:
                        if (PortalBG) PortalBG->ChangeTexture("fs:/vol/content/bakkuguraundo/pretendo.png");
                        color = Colors::Juxt;
                        break;

                    case Inkay::Versions::LocalFileState::Missing:
                        color = Colors::NoInkay;
                        break;

                    default:
                        break;
                }

                if (ColorChangingBars) ColorChangingBars->SetColor(color);

                AlreadyAppliedGUIChange = true;
            }

            if (Inkay::Download::State == DOWNLOADSTATE_SELECT) {
                if (DRC::IsTouchInsideSingle(302.0f, 207.5f, 400.0f, 85.0f)) {
                    Inkay::Download::PendingVersion = "Juxt";
                    Inkay::Download::State.store(DOWNLOADSTATE_SELECT_ENV);
                    Inkay::Dirs::LoadEnvironments();
                } else if (DRC::IsTouchInsideSingle(302.0f, 297.5f, 400.0f, 85.0f)) {
                    Inkay::Download::PendingVersion = "Rose";
                    Inkay::Download::State.store(DOWNLOADSTATE_SELECT_ENV);
                    Inkay::Dirs::LoadEnvironments();
                }

                if (DRC::IsTouchInsideSingle(760.0f, 435.0f, 150.0f, 70.0f)) Inkay::Repos::AboutBrowserURL();
            } else if (Inkay::Download::State == DOWNLOADSTATE_SELECT_ENV) {
                usize envCount = Inkay::Dirs::gEnvironments.size();

                if (envCount > 1) {
                    if (DRC::IsTouchInsideSingle(382.0f, 265.0f, 90.0f, 50.0f)) Inkay::Dirs::gSelectedEnv = (Inkay::Dirs::gSelectedEnv + 1) % envCount; 
                    if (DRC::IsTouchInsideSingle(382.0f, 165.0f, 60.0f, 40.0f)) Inkay::Dirs::gSelectedEnv = (Inkay::Dirs::gSelectedEnv + envCount - 1) % envCount;

                    if (DRC::ButtonTriggered(DRC::Input::A)) {
                        Inkay::Download::SelectedEnvironment = Inkay::Dirs::gEnvironments[Inkay::Dirs::gSelectedEnv];
                        Inkay::Download::State.store(DOWNLOADSTATE_DOWNLOADING);

                        if (Inkay::Download::PendingVersion == "Juxt") Inkay::Download::JuxtDownload();
                        else if (Inkay::Download::PendingVersion == "Rose") Inkay::Download::RoseDownload();
                    }
                } else {
                    Inkay::Download::SelectedEnvironment = Inkay::Dirs::gEnvironments[Inkay::Dirs::gSelectedEnv];
                    Inkay::Download::State.store(DOWNLOADSTATE_DOWNLOADING);

                    if (Inkay::Download::PendingVersion == "Juxt") Inkay::Download::JuxtDownload();
                    else if (Inkay::Download::PendingVersion == "Rose") Inkay::Download::RoseDownload();
                } 

                if (DRC::IsTouchInsideSingle(760.0f, 435.0f, 150.0f, 70.0f)) Inkay::Repos::AboutBrowserURL();
            } else if ((Inkay::Download::State == DOWNLOADSTATE_FINISHED) && (DRC::ButtonTriggered(DRC::Input::B))) Render::FullyRebootConsole();
        }

        void Shutdown(void) {
            if (OSCAFESTDFont) OSCAFESTDFont.reset();
            if (ColorChangingBars) ColorChangingBars.reset();
            if (ButtonsHolder) ButtonsHolder.reset();
            if (ProjectRoseLogo) ProjectRoseLogo.reset();
            if (PretendoLogo) PretendoLogo.reset();
            if (PortalBG) PortalBG.reset();
        }
    }
}