using System.Runtime.InteropServices;

namespace CimpleUI
{
    /* ==== Data Structures ==== */
    [StructLayout(LayoutKind.Sequential)]
    public struct ColorRGBA
    {
        public byte R;
        public byte G;
        public byte B;
        public byte A;

        public ColorRGBA(byte r, byte g, byte b)
        {
            R = r; G = g; B = b; A = 255;
        }
        public ColorRGBA(byte r, byte g, byte b, byte a)
        {
            R = r; G = g; B = b; A = a;
        }

        public static ColorRGBA White => new ColorRGBA(255, 255, 255);
        public static ColorRGBA Black => new ColorRGBA(0, 0, 0);
        public static ColorRGBA Red => new ColorRGBA(255, 0, 0);
        public static ColorRGBA Green => new ColorRGBA(0, 255, 0);
        public static ColorRGBA Blue => new ColorRGBA(0, 0, 255);
        public static ColorRGBA Yellow => new ColorRGBA(255, 255, 0);
        public static ColorRGBA Magenta => new ColorRGBA(255, 0, 255);
        public static ColorRGBA Cyan => new ColorRGBA(0, 255, 255);
        public static ColorRGBA Gray => new ColorRGBA(128, 128, 128);
        public static ColorRGBA DarkGray => new ColorRGBA(64, 64, 64);
        public static ColorRGBA LightGray => new ColorRGBA(192, 192, 192);
        public static ColorRGBA Brown => new ColorRGBA(165, 42, 42);
        public static ColorRGBA Orange => new ColorRGBA(255, 165, 0);
        public static ColorRGBA Pink => new ColorRGBA(255, 192, 203);
        public static ColorRGBA Purple => new ColorRGBA(128, 0, 128);
        public static ColorRGBA Lime => new ColorRGBA(50, 205, 50);
        public static ColorRGBA Navy => new ColorRGBA(0, 0, 128);
        public static ColorRGBA Teal => new ColorRGBA(0, 128, 128);
        public static ColorRGBA Olive => new ColorRGBA(128, 128, 0);
        public static ColorRGBA Maroon => new ColorRGBA(128, 0, 0);
    }

    public enum ButtonState
    {
        Normal,
        Pressed,
        Hovered,
        Released,
        tab,
        tab_active
    }


    public enum TabPannelPossition
    {
        TABPANNEL_TOP,
        TABPANNEL_BUTTOM
    }

    public enum UI_Element
    {
        NULL_ELEM,
        TEXTBOX_ELEM,
        TEXTFIELD_ELEM,
        BUTTON_BASIC_ELEM,
        POPUP_NOTICE_ELEM,
        LABEL_ELEM,
        TABPANNEL_ELEM,
        DROPDOWN_MENU_ELEM
    }

    public enum DropdownMenu_State
    {
        DROPDOWN_NORMAL,
        DROPDOWN_EXPANDED,
        DROPDOWN_SELECTED
    }

    /* ==== Native Imports - P/Invoke internal layer ==== */
    internal static class Native
    {
        const string DLL = "cimple_ui";

        // Initialization
        [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr CimpleUI_Init(byte maxWindows, byte maxFonts);

        // CimpleUI shutdown and SDL quit
        [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void CimpleUI_QuitSDL(IntPtr cimpleUI);

        // Event check
        [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern bool CimpleUI_MultiWindowEventCheck(IntPtr cimpleUI, out ushort windowID);

        // Update UI
        [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void CimpleUI_MultiWindowUIUpdate(IntPtr cimpleUI, float deltaTime);

        // Render UI with clear color and present
        [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void CimpleUI_MultiWindowRender(IntPtr cimpleUI, ColorRGBA color);

        // Window creating 
        [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr CimpleUI_CreateWindowController(IntPtr cimpleUI,
        [MarshalAs(UnmanagedType.LPStr)] string title, uint width, uint height, short uiElemMax);

        // Destroy WindowController
        [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void CimpleUI_DestroyWindowController(IntPtr cimpleUI, IntPtr windowController);

        // Get window width and height
        [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern uint CimpleUI_GetWindowWidth(IntPtr windowController);
        [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern uint CimpleUI_GetWindowHeight(IntPtr windowController);

        // Loading font
        [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void CimpleUI_LoadFont(IntPtr cimpleUI,
        [MarshalAs(UnmanagedType.LPStr)] string fileName, byte fontSize);

        // Create Label
        [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr CimpleUI_CreateLabel(IntPtr windowController, int x, int y, int width, int height,
                [MarshalAs(UnmanagedType.LPStr)] string text, byte fontIndex, byte fontSize, ColorRGBA color);


        // Callback delegate 
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void ButtonClickCallback(IntPtr userData);


        // Textbox functions
        [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr CimpleUI_CreateTextBox(
            IntPtr windowController,
            byte fontIndex,
            byte fontSize,
            ColorRGBA color,
            float x, float y, float width, float height);

        [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void CimpleUI_TextBoxAppendText(
            IntPtr windowController, IntPtr textbox, [MarshalAs(UnmanagedType.LPStr)] string text);

        [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void CimpleUI_TextBoxGetText(
            IntPtr textbox, byte[] dest);


        [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern uint CimpleUI_TextBoxGetTextLength(IntPtr textbox);

        [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void CimpleUI_TextBox_Clear(IntPtr textbox, IntPtr cimpleUI);

        // TextField functions
        [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr CimpleUI_CreateTextField(
            IntPtr windowController,
            byte fontIndex,
            byte fontSize,
            ColorRGBA color,
            float x, float y, float width, float height);

        [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void CimpleUI_TextFieldAppendText(
            IntPtr windowController, IntPtr textfield, [MarshalAs(UnmanagedType.LPStr)] string text);

        [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void CimpleUI_TextFieldGetText(
            IntPtr textfield, byte[] dest);

        [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern uint CimpleUI_TextFieldGetTextLength(IntPtr textfield);

        [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void CimpleUI_TextField_Clear(IntPtr textfield, IntPtr cimpleUI);


        // Button functions
        [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr CimpleUI_CreateButton(
            IntPtr windowController,
            byte fontIndex,
            byte fontSize,
            int x, int y, int width, int height,
            [MarshalAs(UnmanagedType.LPStr)] string text,
            ColorRGBA color);

        [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void CimpleUI_ButtonAddClickListener(IntPtr windowController, IntPtr button,
           ButtonClickCallback callback, IntPtr userData);

        [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern int CimpleUI_ButtonGetState(IntPtr button);


        // TabPannel functions
        [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr CimpleUI_CreateTabPannel(
            IntPtr windowController,
            [MarshalAs(UnmanagedType.LPStr)] string tabNames,
            TabPannelPossition possition,
            byte fontIndex,
            byte fontSize,
            int height,
            ColorRGBA color, int elemPerTab);

        [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void CimpleUI_AddElementToTabPannel(
            IntPtr tabPannel, IntPtr elem, UI_Element uiElement, int tab);


        //dropdown menu
        [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr CimpleUI_dropdown_menu_init(
            IntPtr windowController, byte maxCount, [MarshalAs(UnmanagedType.LPStr)] string label,
            byte fontIndex, byte fontSize,
            int x, int y, int w, int h, ColorRGBA color);

        [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void CimpleUI_dropdown_menu_populate(
            IntPtr windowController, IntPtr ddm,
            [MarshalAs(UnmanagedType.LPStr)] string textString, byte fontIndex, byte fontSize, ColorRGBA color);

        [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void CimpleUI_dropdown_menu_add_listener(
            IntPtr windowController, IntPtr ddm, ButtonClickCallback callback,
            IntPtr userData);

        [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern int CimpleUI_dropdown_button_selected(IntPtr ddm);

        [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void CimpleUI_reset_dropdown_menu(IntPtr ddm);

        [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void CimpleUI_select_dropdown_menu_button(IntPtr ddm, byte buttonIndex);

        [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern int CimpleUI_dropdown_menu_get_state(IntPtr ddm);


        // Popup Notice
        [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void CimpleUI_PopupNoticeInit(
                IntPtr windowController, [MarshalAs(UnmanagedType.LPStr)] string noticeText,
                [MarshalAs(UnmanagedType.LPStr)] string buttonText,
                byte fontIndex, int width, int height, ColorRGBA color);


    }

    // ============ High-Level API (Class wrapper) ============

    // IDisposable means, if use with 'using' statement, will use use dispose when no longer needed

    public class CimpleUIController : IDisposable
    {
        private IntPtr _handle;

        public CimpleUIController(byte maxWindows = 4, byte maxFonts = 5)
        {
            _handle = Native.CimpleUI_Init(maxWindows, maxFonts);
            if (_handle == IntPtr.Zero)
                throw new Exception("Failed to initialize CimpleUI");
        }

        internal IntPtr Handle => _handle;

        public bool Event()
        {
            if (Native.CimpleUI_MultiWindowEventCheck(_handle, out ushort index))
                return true;
            else
                return false;
        }

        public bool Event(out short WindowIndex)
        {
            if (Native.CimpleUI_MultiWindowEventCheck(_handle, out ushort index))
            {
                WindowIndex = (short)index;
                return true;
            }
            else
            {
                WindowIndex = -1;
                return false;
            }

        }

        public void Update(float deltaTime) => Native.CimpleUI_MultiWindowUIUpdate(_handle, deltaTime);

        public void Render(ColorRGBA color) => Native.CimpleUI_MultiWindowRender(_handle, color);

        public void LoadFont(string fileName, byte fontSize = 18)
        {
            Native.CimpleUI_LoadFont(_handle, fileName, fontSize);
        }

        public void Dispose()
        {
            if (_handle != IntPtr.Zero)
            {
                Native.CimpleUI_QuitSDL(_handle);
                _handle = IntPtr.Zero;
            }
        }
    }

    public class WindowController : IDisposable
    {
        private IntPtr _handle;
        private CimpleUIController uiController;

        public WindowController(CimpleUIController ui, string title, uint width = 1000, uint height = 800, short uiElem = 64)
        {
            if (ui == null) throw new ArgumentNullException(nameof(ui));

            _handle = Native.CimpleUI_CreateWindowController(ui.Handle, title, width, height, uiElem);
            uiController = ui;
        }

        internal IntPtr Handle => _handle;
        internal CimpleUIController UIController => uiController;

        public uint Height => Native.CimpleUI_GetWindowHeight(_handle);
        public uint Width => Native.CimpleUI_GetWindowWidth(_handle);

        public void Dispose()
        {
            Native.CimpleUI_DestroyWindowController(uiController.Handle, _handle);
        }

    }

    public class Label
    {
        private IntPtr _handle;

        public Label(WindowController window, int x, int y, int width, int height, string text, TabPannel? tp = null, int? tab = null,
           byte fontIndex = 0, byte fontSize = 0, ColorRGBA color = default)
        {
            if (window == null) throw new ArgumentNullException(nameof(window));
            if (string.IsNullOrWhiteSpace(text)) throw new ArgumentException("text is null or only white space");

            if (color.R == 0 && color.G == 0 && color.B == 0 && color.A == 0)
                color = ColorRGBA.Blue;

            _handle = Native.CimpleUI_CreateLabel(
                    window.Handle, x, y, width, height, text, fontIndex, fontSize, color);

            if (_handle == IntPtr.Zero)
                throw new Exception("Failed to create label");

            if (tp != null && tab != null)
            {
                TabPannel.add_elem(_handle, tp.Handle, UI_Element.LABEL_ELEM, (int)tab);
            }
        }
        internal IntPtr Handle => _handle;
    }

    public class TextBox
    {
        private IntPtr _handle;
        private WindowController Window;

        public TextBox(WindowController window, float x, float y, float width, float height, TabPannel? tp = null, int? tab = null,
            byte fontIndex = 0, byte fontSize = 20, ColorRGBA color = default)
        {
            if (window == null) throw new ArgumentNullException(nameof(window));

            if (color.R == 0 && color.G == 0 && color.B == 0 && color.A == 0)
                color = ColorRGBA.White;

            _handle = Native.CimpleUI_CreateTextBox(
                    window.Handle, fontIndex, fontSize, color, x, y, width, height);

            if (_handle == IntPtr.Zero)
                throw new Exception("Failed to create textbox");

            Window = window;

            if (tp != null && tab != null)
            {
                TabPannel.add_elem(_handle, tp.Handle, UI_Element.TEXTBOX_ELEM, (int)tab);
            }
        }

        internal IntPtr Handle => _handle;

        public void AppendText(string text)
        {
            if (text == null) throw new ArgumentNullException(nameof(text));
            Native.CimpleUI_TextBoxAppendText(Window.Handle, _handle, text);
        }


        public string ToText()
        {
            int length = (int)Native.CimpleUI_TextBoxGetTextLength(_handle);
            byte[] tmp = new byte[length];
            Native.CimpleUI_TextBoxGetText(_handle, tmp);
            return System.Text.Encoding.UTF8.GetString(tmp, 0, length - 1);
        }

        public void Clear()
        {
            Native.CimpleUI_TextBox_Clear(_handle, Window.UIController.Handle);
        }
    }


    public class TextField
    {
        private IntPtr _handle;
        private WindowController Window;

        public TextField(WindowController window, float x, float y, float width, float height, TabPannel? tp = null, int? tab = null,
            byte fontIndex = 0, byte fontSize = 20, ColorRGBA color = default)
        {
            if (window == null) throw new ArgumentNullException(nameof(window));

            if (color.R == 0 && color.G == 0 && color.B == 0 && color.A == 0)
                color = ColorRGBA.White;

            _handle = Native.CimpleUI_CreateTextField(
                    window.Handle, fontIndex, fontSize, color, x, y, width, height);

            if (_handle == IntPtr.Zero)
                throw new Exception("Failed to create textfield");

            Window = window;

            if (tp != null && tab != null)
            {
                TabPannel.add_elem(_handle, tp.Handle, UI_Element.TEXTFIELD_ELEM, (int)tab);
            }
        }

        internal IntPtr Handle => _handle;

        public void AppendText(string text)
        {
            if (text == null) throw new ArgumentNullException(nameof(text));
            Native.CimpleUI_TextFieldAppendText(Window.Handle, _handle, text);
        }


        public string ToText()
        {
            int length = (int)Native.CimpleUI_TextFieldGetTextLength(_handle);
            byte[] tmp = new byte[length];
            Native.CimpleUI_TextFieldGetText(_handle, tmp);
            return System.Text.Encoding.UTF8.GetString(tmp, 0, length - 1);
        }

        public void Clear()
        {
            Native.CimpleUI_TextField_Clear(_handle, Window.UIController.Handle);
        }
    }

    public class Button
    {
        private IntPtr _handle;
        private Native.ButtonClickCallback _nativeCallback;
        private List<Action> _eventHandlers = new List<Action>();

        //Adding the Clicked event
        public event Action? Clicked
        {
            add
            {
                if (value != null)
                    _eventHandlers.Add(value);
            }
            remove
            {
                if (value != null)
                    _eventHandlers.Remove(value);
            }
        }

        public Button(WindowController window, int x, int y, int width, int height, string text, TabPannel? tp = null, int? tab = null,
            byte fontIndex = 0, byte fontSize = 0, ColorRGBA color = default)
        {
            if (window == null) throw new ArgumentNullException(nameof(window));

            if (color.R == 0 && color.G == 0 && color.B == 0 && color.A == 0)
                color = ColorRGBA.Green;

            _handle = Native.CimpleUI_CreateButton(
                    window.Handle, fontIndex, fontSize, x, y, width, height, text ?? string.Empty, color);

            if (_handle == IntPtr.Zero)
                throw new Exception("Failed to create button");

            // Setup native callback - don't need to worry about Pointers for data and we can capture it from the lambda event declaration
            _nativeCallback = OnNativeClick;
            Native.CimpleUI_ButtonAddClickListener(window.Handle, _handle, _nativeCallback, IntPtr.Zero);

            if (tp != null && tab != null)
            {
                TabPannel.add_elem(_handle, tp.Handle, UI_Element.BUTTON_BASIC_ELEM, (int)tab);
            }
        }

        internal IntPtr Handle => _handle;

        public ButtonState State => (ButtonState)Native.CimpleUI_ButtonGetState(_handle);

        private void OnNativeClick(IntPtr userData)
        {
            foreach (var handler in _eventHandlers)
            {
                handler?.Invoke();
            }
        }
    }
    public class TabPannel
    {
        private IntPtr _handle;

        //tabe names like "tab1|tab2|tab3|etc..."
        public TabPannel(WindowController window, string tabNames, TabPannelPossition possition = TabPannelPossition.TABPANNEL_TOP,
            int height = 50, byte fontIndex = 0, byte fontSize = 0, ColorRGBA color = default, int elemPerTab = 24)
        {
            if (window == null) throw new ArgumentNullException(nameof(window));

            if (color.R == 0 && color.G == 0 && color.B == 0 && color.A == 0)
                color = ColorRGBA.DarkGray;

            _handle = Native.CimpleUI_CreateTabPannel(window.Handle, tabNames, possition, fontIndex, fontSize, height, color, elemPerTab);
        }

        internal IntPtr Handle => _handle;

        public static void add_elem(IntPtr elem, IntPtr tabPannel, UI_Element uiElementType, int tab)
        {
            Native.CimpleUI_AddElementToTabPannel(tabPannel, elem, uiElementType, tab);
        }

    }

    public class DropdownMenu
    {
        private IntPtr _handle;
        private WindowController Window;
        private Native.ButtonClickCallback _nativeCallback;
        private List<Action> _eventHandlers = new List<Action>();

        //Adding the Selected event
        public event Action? Selected
        {
            add
            {
                if (value != null)
                    _eventHandlers.Add(value);
            }
            remove
            {
                if (value != null)
                    _eventHandlers.Remove(value);
            }
        }

        public DropdownMenu(WindowController window, string label,
            int x, int y, int width, int height, TabPannel? tp = null, int? tab = null, byte maxCount = 32,
            byte fontIndex = 0, byte fontSize = 0, ColorRGBA color = default)
        {
            if (window == null) throw new ArgumentNullException(nameof(window));

            if (color.R == 0 && color.G == 0 && color.B == 0 && color.A == 0)
                color = ColorRGBA.Black;

            _handle = Native.CimpleUI_dropdown_menu_init(
                    window.Handle, maxCount, label ?? string.Empty,
                fontIndex, fontSize, x, y, width, height, color);

            Window = window;

            // Setup native callback
            _nativeCallback = OnNativeSelected;
            Native.CimpleUI_dropdown_menu_add_listener(window.Handle, _handle, _nativeCallback, IntPtr.Zero);

            if (tp != null && tab != null)
            {
                TabPannel.add_elem(_handle, tp.Handle, UI_Element.DROPDOWN_MENU_ELEM, (int)tab);
            }
        }

        internal IntPtr Handle => _handle;

        public int SelectedIndex => Native.CimpleUI_dropdown_button_selected(_handle);

        public DropdownMenu_State State => (DropdownMenu_State)Native.CimpleUI_dropdown_menu_get_state(_handle);

        public void Reset() => Native.CimpleUI_reset_dropdown_menu(_handle);

        public void Select(byte index) => Native.CimpleUI_select_dropdown_menu_button(_handle, index);

        //Each element is seperated by '\n'
        public void Populate(string textString,
            byte fontIndex = 0, byte fontSize = 0, ColorRGBA color = default)
        {
            if (color.R == 0 && color.G == 0 && color.B == 0 && color.A == 0)
                color = ColorRGBA.Black;

            if (!string.IsNullOrWhiteSpace(textString))
                Native.CimpleUI_dropdown_menu_populate(Window.Handle, _handle,
                    textString, fontIndex, fontSize, color);
        }

        private void OnNativeSelected(IntPtr userData)
        {
            foreach (var handler in _eventHandlers)
            {
                handler?.Invoke();
            }
        }
    }

    public static class PopupNotice
    {
        public static void Create(WindowController window, string notice, string buttonText,
            byte fontIndex = 0, int width = 300, int height = 150, ColorRGBA color = default)
        {
            if (window == null) throw new ArgumentNullException(nameof(window));

            if (color.R == 0 && color.G == 0 && color.B == 0 && color.A == 0)
                color = ColorRGBA.White;

            Native.CimpleUI_PopupNoticeInit(
                window.Handle, notice ?? string.Empty, buttonText ?? string.Empty,
                fontIndex, width, height, color);
        }
    }
}
