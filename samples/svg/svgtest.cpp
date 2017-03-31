/////////////////////////////////////////////////////////////////////////////
// Name:        svgtest.cpp
// Purpose:     SVG sample
// Author:      Chris Elliott
// Modified by:
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ===========================================================================
// declarations
// ===========================================================================

// ---------------------------------------------------------------------------
// headers
// ---------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/dcsvg.h"
#include "wx/notebook.h"

#include "SVGlogo24.xpm"

#ifndef wxHAS_IMAGES_IN_RESOURCES
    #include "../sample.xpm"
#endif

#include <math.h>

// ---------------------------------------------------------------------------
// classes
// ---------------------------------------------------------------------------

class MyApp : public wxApp
{
public:
    bool OnInit() wxOVERRIDE;
};

class MyPage : public wxScrolledWindow
{
public:
    MyPage(wxNotebook *parent, int index);
    virtual void OnDraw(wxDC& dc) wxOVERRIDE;
    bool OnSave(wxString);
private:
    int m_index;
};

class MyFrame : public wxFrame
{
public:
    MyFrame(wxWindow *parent, const wxWindowID id, const wxString& title,
            const wxPoint& pos, const wxSize& size);

    MyPage *CreateNewPage(int index);

    void FileSavePicture(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnQuit(wxCommandEvent& event);
private:
    wxNotebook *m_notebook;
    wxPanel *m_panel;
    wxBoxSizer *m_sizerFrame;

    wxDECLARE_EVENT_TABLE();
};

// ---------------------------------------------------------------------------
// event tables
// ---------------------------------------------------------------------------

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)
    EVT_MENU(wxID_EXIT, MyFrame::OnQuit)
    EVT_MENU(wxID_SAVE, MyFrame::FileSavePicture)
wxEND_EVENT_TABLE()

// ===========================================================================
// implementation
// ===========================================================================

// ---------------------------------------------------------------------------
// MyApp
// ---------------------------------------------------------------------------

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    // Create the main frame window

    MyFrame* frame = new MyFrame(NULL, -1, wxT("SVG Demo"),
                                 wxDefaultPosition, wxSize(500, 400));

    frame->Show(true);

    return true;
}

// ---------------------------------------------------------------------------
// MyFrame
// ---------------------------------------------------------------------------

// Define my frame constructor

MyFrame::MyFrame(wxWindow *parent, const wxWindowID id, const wxString& title,
                 const wxPoint& pos, const wxSize& size)
        : wxFrame(parent, id, title, pos, size)
{
    SetIcon(wxICON(sample));

    #if wxUSE_STATUSBAR
    CreateStatusBar();
    #endif // wxUSE_STATUSBAR

    // Make a menubar
    wxMenu *file_menu = new wxMenu;

    file_menu->Append(wxID_SAVE);
    file_menu->Append(wxID_EXIT);

    wxMenu *help_menu = new wxMenu;
    help_menu->Append(wxID_ABOUT);

    wxMenuBar *menu_bar = new wxMenuBar;

    menu_bar->Append(file_menu, wxT("&File"));
    menu_bar->Append(help_menu, wxT("&Help"));

    // Associate the menu bar with the frame
    SetMenuBar(menu_bar);

    // Create a notebook
    m_notebook = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBK_TOP);

    //Add SVG Windows to a notebook
    for (int i = 0; i <= 8; ++i)
    {
        wxString svgTitle;
        svgTitle.Printf(wxT("SVG Test %d"), i);
        m_notebook->AddPage(CreateNewPage(i), svgTitle);

    }
}

MyPage *MyFrame::CreateNewPage(int index)
{
    MyPage *page = new MyPage(m_notebook, index);
    return page;
}

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Close();
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    (void)wxMessageBox(wxT("wxWidgets SVG sample\n")
        wxT("Author: Chris Elliott (c) 2002-2009\n")
        wxT("Usage: click File|Save to Save the Selected SVG Test"),
        wxT("About SVG Test"));
}

void MyFrame::FileSavePicture(wxCommandEvent& WXUNUSED(event))
{
#if wxUSE_FILEDLG
    MyPage *page = (MyPage *) m_notebook->GetCurrentPage();
    if (page == NULL)
    {
        return;
    }

    wxFileDialog dialog(this, wxT("Save Picture as"), wxEmptyString,
        m_notebook->GetPageText(m_notebook->GetSelection()),
        wxT("SVG vector picture files (*.svg)|*.svg"),
        wxFD_SAVE|wxFD_OVERWRITE_PROMPT);

    if (dialog.ShowModal() == wxID_OK)
    {
        if (!page->OnSave ( dialog.GetPath() ))
        {
            return;
        }
    }
    return;
#endif // wxUSE_FILEDLG
}

// ---------------------------------------------------------------------------
// MyPage
// ---------------------------------------------------------------------------

// Define a constructor for my page
MyPage::MyPage(wxNotebook *parent, int index)
    : wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL|wxHSCROLL)
{
    SetBackgroundColour(*wxWHITE);
    SetScrollbars(20, 20, 50, 50);
    m_index = index;
}

bool MyPage::OnSave(wxString filename)
{
    wxSVGFileDC svgDC (filename, 600, 650);
    OnDraw (svgDC);
    return svgDC.IsOk();
}

// Define the repainting behaviour
void MyPage::OnDraw(wxDC& dc)
{
     // vars to use ...
    wxString s;
    wxPen wP;
    wxBrush wB;
    wxPoint points[6];
    wxColour wC;

    dc.SetFont(*wxSWISS_FONT);
    dc.SetPen(*wxGREEN_PEN);

    switch (m_index)
    {
        default:
        case 0:
            // draw lines to make a cross
            dc.DrawLine(0, 0, 200, 200);
            dc.DrawLine(200, 0, 0, 200);
            // draw point colored line and spline
            wP = *wxCYAN_PEN;
            wP.SetWidth(3);
            dc.SetPen(wP);

            dc.DrawPoint (25,15);
            dc.DrawLine(50, 30, 200, 30);
            dc.DrawSpline(50, 200, 50, 100, 200, 10);
            s = wxT("Green Cross, Cyan Line and spline");
            break;

        case 1:
            // draw standard shapes
            dc.SetBrush(*wxCYAN_BRUSH);
            dc.SetPen(*wxRED_PEN);
            dc.DrawRectangle(10, 10, 100, 70);
            wB = wxBrush (wxT("DARK ORCHID"), wxBRUSHSTYLE_TRANSPARENT);
            dc.SetBrush (wB);
            dc.DrawRoundedRectangle(50, 50, 100, 70, 20);
            dc.SetBrush (wxBrush(wxT("GOLDENROD")) );
            dc.DrawEllipse(100, 100, 100, 50);

            points[0].x = 100; points[0].y = 200;
            points[1].x = 70; points[1].y = 260;
            points[2].x = 160; points[2].y = 230;
            points[3].x = 40; points[3].y = 230;
            points[4].x = 130; points[4].y = 260;
            points[5].x = 100; points[5].y = 200;

            dc.DrawPolygon(5, points);
            dc.DrawLines (6, points, 160);
            s = wxT("Blue rectangle, red edge, clear rounded rectangle, gold ellipse, gold and clear stars");
            break;

        case 2:
            // draw text in Arial or similar font
            dc.DrawLine(50,25,50,35);
            dc.DrawLine(45,30,55,30);
            dc.DrawText(wxT("This is a Swiss-style string"), 50, 30);
            wC = dc.GetTextForeground();
            dc.SetTextForeground (wxT("FIREBRICK"));

            // no effect in msw ??
            dc.SetTextBackground (wxT("WHEAT"));
            dc.DrawText(wxT("This is a Red string"), 50, 200);
            dc.DrawRotatedText(wxT("This is a 45 deg string"), 50, 200, 45);
            dc.DrawRotatedText(wxT("This is a 90 deg string"), 50, 200, 90);
            dc.SetFont(wxFontInfo(18)
                        .FaceName("Times New Roman")
                        .Family(wxFONTFAMILY_ROMAN)
                        .Italic().Bold());
            dc.SetTextForeground (wC);
            dc.DrawText(wxT("This is a Times-style string"), 50, 60);
            s = wxT("Swiss, Times text; red text, rotated and colored orange");
            break;

        case 3 :
            // four arcs start and end points, center
            dc.SetBrush(*wxGREEN_BRUSH);
            dc.DrawArc ( 200,300, 370,230, 300,300 );
            dc.SetBrush(*wxBLUE_BRUSH);
            dc.DrawArc ( 270-50, 270-86, 270-86, 270-50, 270,270 );
            dc.SetDeviceOrigin(-10,-10);
            dc.DrawArc ( 270-50, 270-86, 270-86, 270-50, 270,270 );
            dc.SetDeviceOrigin(0,0);

            wP.SetColour (wxT("CADET BLUE"));
            dc.SetPen(wP);
            dc.DrawArc ( 75,125, 110, 40, 75, 75 );

            wP.SetColour (wxT("SALMON"));
            dc.SetPen(wP);
            dc.SetBrush(*wxRED_BRUSH);
            //top left corner, width and height, start and end angle
                                 // 315 same center and x-radius as last pie-arc, half Y radius
            dc.DrawEllipticArc(25,50,100,50,180.0,45.0);

            wP = *wxCYAN_PEN;
            wP.SetWidth(3);
            dc.SetPen(wP);
                                 //wxTRANSPARENT));
            dc.SetBrush (wxBrush (wxT("SALMON")));
            dc.DrawEllipticArc(300,  0,200,100, 0.0,145.0);
                                 //same end point
            dc.DrawEllipticArc(300, 50,200,100,90.0,145.0);
            dc.DrawEllipticArc(300,100,200,100,90.0,345.0);

            s = wxT("This is an arc test page");
            break;

        case 4:
            dc.DrawCheckMark ( 30,30,25,25);
            dc.SetBrush (wxBrush (wxT("SALMON"),wxBRUSHSTYLE_TRANSPARENT));
            dc.DrawCheckMark ( 80,50,75,75);
            dc.DrawRectangle ( 80,50,75,75);
            s = wxT("Two check marks");
            break;

        case 5:
            dc.SetFont(wxFontInfo(18)
                        .FaceName("Times New Roman")
                        .Family(wxFONTFAMILY_ROMAN)
                        .Italic().Bold());
            dc.DrawLine(0, 0, 200, 200);
            dc.DrawLine(200, 0, 0, 200);
            dc.DrawText(wxT("This is an 18pt string"), 50, 60);

            // rescale and draw in blue
            wP = *wxCYAN_PEN;
            dc.SetPen(wP);
            dc.SetUserScale (2.0,0.5);
            dc.SetDeviceOrigin(200,0);
            dc.DrawLine(0, 0, 200, 200);
            dc.DrawLine(200, 0, 0, 200);
            dc.DrawText(wxT("This is an 18pt string 2 x 0.5 UserScaled"), 50, 60);
            dc.SetUserScale (2.0,2.0);
            dc.SetDeviceOrigin(200,200);
            dc.DrawText(wxT("This is an 18pt string 2 x 2 UserScaled"), 50, 60);

            wP = *wxRED_PEN;
            dc.SetPen(wP);
            dc.SetUserScale (1.0,1.0);
            dc.SetDeviceOrigin(0,10);
            dc.SetMapMode (wxMM_METRIC); //svg ignores this
            dc.DrawLine(0, 0, 200, 200);
            dc.DrawLine(200, 0, 0, 200);
            dc.DrawText(wxT("This is an 18pt string in MapMode"), 50, 60);
            s = wxT("Scaling test page");
            break;

        case 6:
            dc.DrawIcon( wxICON(sample), 10, 10 );
            dc.DrawBitmap ( wxBitmap(svgbitmap_xpm), 50,15);
            s = wxT("Icon and Bitmap ");
            break;

        case 7:
            dc.SetTextForeground(wxT("RED"));
            dc.DrawText(wxT("Red = Clipping Off"), 30, 5);
            dc.SetTextForeground(wxT("GREEN"));
            dc.DrawText(wxT("Green = Clipping On"), 30, 25);

            dc.SetTextForeground(wxT("BLACK"));

            dc.SetPen(*wxRED_PEN);
            dc.SetBrush (wxBrush (wxT("SALMON"),wxBRUSHSTYLE_TRANSPARENT));
            dc.DrawCheckMark ( 80,50,75,75);
            dc.DrawRectangle ( 80,50,75,75);

            dc.SetPen(*wxGREEN_PEN);

            // Clipped checkmarks
            dc.DrawRectangle(180,50,75,75);
            dc.SetClippingRegion(180,50,75,75);                   // x,y,width,height version
            dc.DrawCheckMark ( 180,50,75,75);
            dc.DestroyClippingRegion();

            dc.DrawRectangle(wxRect(80,150,75,75));
            dc.SetClippingRegion(wxPoint(80,150),wxSize(75,75));  // pt,size version
            dc.DrawCheckMark ( 80,150,75,75);
            dc.DestroyClippingRegion();

            dc.DrawRectangle(wxRect(180,150,75,75));
            dc.SetClippingRegion(wxRect(180,150,75,75));          // rect version
            dc.DrawCheckMark ( 180,150,75,75);
            dc.DestroyClippingRegion();

            dc.DrawRectangle(wxRect( 80,250,50,65));
            dc.DrawRectangle(wxRect(105,260,50,65));
            dc.SetClippingRegion(wxRect( 80,250,50,65));  // second call to SetClippingRegion
            dc.SetClippingRegion(wxRect(105,260,50,65));  // forms intersection with previous
            dc.DrawCheckMark(80,250,75,75);
            dc.DestroyClippingRegion();                   // only one call to destroy (there's no stack)

            /*
            ** Clipping by wxRegion not implemented for SVG.   Should be
            ** possible, but need to access points that define the wxRegion
            ** from inside DoSetDeviceClippingRegion() and wxRegion does not
            ** implement anything like getPoints().
            points[0].x = 180; points[0].y = 250;
            points[1].x = 255; points[1].y = 250;
            points[2].x = 180; points[2].y = 325;
            points[3].x = 255; points[3].y = 325;
            points[4].x = 180; points[4].y = 250;

            dc.DrawLines (5, points);
            wxRegion reg = wxRegion(5,points);

            dc.SetClippingRegion(reg);
            dc.DrawCheckMark ( 180,250,75,75);
            dc.DestroyClippingRegion();
            */

            s = wxT("Clipping region");
            break;

        case 8:
            wxString txtStr;
            wxCoord txtX, txtY, txtW, txtH, txtDescent, txtEL;
            wxCoord txtPad = 0;

            wP = *wxRED_PEN;
            dc.SetPen(wP);
            //dc.SetBackgroundMode(wxBRUSHSTYLE_SOLID);
            //dc.SetTextBackground(*wxBLUE);

            // Horizontal text
            txtStr = wxT("Horizontal string");
            dc.GetTextExtent(txtStr, &txtW, &txtH, &txtDescent, &txtEL);
            txtX = 50;
            txtY = 300;
            dc.DrawRectangle(txtX, txtY, txtW + 2*txtPad, txtH + 2*txtPad);
            dc.DrawText(txtStr, txtX + txtPad, txtY + txtPad);

            // Vertical text
            txtStr = wxT("Vertical string");
            dc.GetTextExtent(txtStr, &txtW, &txtH, &txtDescent, &txtEL);
            txtX = 50;
            txtY = 250;
            dc.DrawRectangle(txtX, txtY - (txtW + 2*txtPad), txtH + 2*txtPad, txtW + 2*txtPad);
            dc.DrawRotatedText(txtStr, txtX + txtPad, txtY - txtPad, 90);

            // 45 degree text
            txtStr = wxT("45 deg string");
            dc.GetTextExtent(txtStr, &txtW, &txtH, &txtDescent, &txtEL);
            double lenW = (double)(txtW + 2*txtPad) / sqrt(2.0);
            double lenH = (double)(txtH + 2*txtPad) / sqrt(2.0);
            double padding = (double)txtPad / sqrt(2.0);
            txtX = 150;
            txtY = 200;
            dc.DrawLine(txtX - padding, txtY, txtX + lenW, txtY - lenW); // top
            dc.DrawLine(txtX + lenW, txtY - lenW, txtX - padding + lenH + lenW, txtY + (lenH - lenW));
            dc.DrawLine(txtX - padding, txtY, txtX - padding + lenH, txtY + lenH);
            dc.DrawLine(txtX - padding + lenH, txtY + lenH, txtX - padding + lenH + lenW, txtY + (lenH - lenW)); // bottom
            dc.DrawRotatedText(txtStr, txtX, txtY, 45);
            s = wxT("Text position test page");
            break;
    }

   wxLogStatus("%s", s);
}

