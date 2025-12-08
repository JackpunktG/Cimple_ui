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
        BUTTON_BASIC_ELEM,
        POPUP_NOTICE_ELEM,
        LABEL_ELEM,
        TABPANNEL_ELEM
    }

    /* ==== Native Imports - P/Invoke internal layer ==== */
    internal static class Native
    {
        const string DLL = "cimple_ui";

        [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr CimpleUI_InitWindow(IntPtr arena,
            [MarshalAs(UnmanagedType.LPStr)] string title,
            uint width, uint height, bool vsync, bool fullscreen);

        [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void CimpleUI_DestroyWindow(IntPtr window);

        [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern bool CimpleUI_PollEvent(IntPtr window);

        [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void CimpleUI_ClearScreen(IntPtr window, ColorRGBA color);

        [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void CimpleUI_Present(IntPtr window);

        [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern uint CimpleUI_GetWindowWidth(IntPtr window);

        [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern uint CimpleUI_GetWindowHeight(IntPtr window);

        [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr CimpleUI_CreateArena();

        [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void CimpleUI_DestroyStringMemory(IntPtr sm);

        [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern bool CimpleUI_event_check(IntPtr window, IntPtr arena, IntPtr sm, IntPtr uiC);

        [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void CimpleUI_DestroyArena(IntPtr arena);

        [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr CimpleUI_CreateStringMemory(IntPtr arena, ushort maxStrings);

        [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr CimpleUI_CreateFontHolder(IntPtr arena, byte maxFonts);

        [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void CimpleUI_LoadFont(IntPtr fh,
            [MarshalAs(UnmanagedType.LPStr)] string fileName, byte fontSize);

        [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr CimpleUI_CreateLabel(IntPtr window,
            IntPtr arena, IntPtr uiController, int x, int y, int width, int hight,
            [MarshalAs(UnmanagedType.LPStr)] string text, IntPtr fh, byte fontIndex, byte fontSize, ColorRGBA color);

        [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void CimpleUI_DestroyFonts(IntPtr fh);

        [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr CimpleUI_CreateUIController(IntPtr arena, ushort totalElements);

        [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void CimpleUI_UpdateUI(IntPtr uiC, float deltaTime);

        [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void CimpleUI_RenderUI(IntPtr window, IntPtr uiC);

        [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void CimpleUI_DestroyUIController(IntPtr uiC);

        [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr CimpleUI_CreateTextBox(
            IntPtr arena, IntPtr uiController, IntPtr sm,
            IntPtr fh, byte fontIndex, byte fontSize,
            ColorRGBA color, float x, float y, float width, float height);

        [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void CimpleUI_TextBoxAppendText(IntPtr arena, IntPtr sm, IntPtr textbox,
            [MarshalAs(UnmanagedType.LPStr)] string text);

        [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void CimpleUI_TextBoxGetText(IntPtr textbox, byte[] dest);

        [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern uint CimpleUI_TextBoxGetTextLength(IntPtr textbox);

        [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr CimpleUI_CreateButton(
            IntPtr arena, IntPtr uiController, IntPtr window,
            IntPtr fh, byte fontIndex, byte fontSize,
            int x, int y, int width, int height,
            [MarshalAs(UnmanagedType.LPStr)] string text,
            ColorRGBA color);

        //main arena is the arena used to create the string memory contoller
        [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr CimpleUI_TextBox_Clear(IntPtr textbox, IntPtr sm, IntPtr mainArena);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void ButtonClickCallback(IntPtr userData);

        [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void CimpleUI_ButtonAddClickListener(IntPtr arena, IntPtr button,
            ButtonClickCallback callback, IntPtr userData);

        [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern int CimpleUI_ButtonGetState(IntPtr button);

        [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void CimpleUI_PopupNoticeInit(
           IntPtr uiController, [MarshalAs(UnmanagedType.LPStr)] string notice, [MarshalAs(UnmanagedType.LPStr)] string button,
           IntPtr fh, byte fontIndex, int width, int height, ColorRGBA color);

        [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr CimpleUI_CreateTabPannel(
            IntPtr arena, IntPtr uiController, IntPtr window, [MarshalAs(UnmanagedType.LPStr)] string tabNames,
            TabPannelPossition possition, IntPtr fh, byte fontIndex, byte fontSize, int height, ColorRGBA color, int elemPerTab);

        [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)]
        public static extern void CimpleUI_AddElementToTabPannel(
            IntPtr tabPannel, IntPtr elem, UI_Element uiElement, int tab);

        /*public static string textbox_gettext(intptr textbox)
        {
            int length = (int)cimpleui_textboxgettextlength(textbox);
            byte[] tmp = new byte[length];
            cimpleui_textboxgettext(textbox, tmp);
            return system.text.encoding.utf8.getstring(tmp, 0, length - 1);
        }*/
    }


    // ============ High-Level API (Class wrapper) ============

    // IDisposable means, if used with 'using' statement, will auto dispose else need manual dispose

    public class Arena : IDisposable
    {
        private IntPtr _handle;

        public Arena()
        {
            _handle = Native.CimpleUI_CreateArena();
            if (_handle == IntPtr.Zero)
                throw new Exception("Failed to create arena");
        }

        internal IntPtr Handle => _handle;

        public void Dispose()
        {
            if (_handle != IntPtr.Zero)
            {
                Native.CimpleUI_DestroyArena(_handle);
                _handle = IntPtr.Zero;
            }
        }
    }

    public class Window : IDisposable
    {
        private IntPtr _handle;
        private Arena _arena;
        internal IntPtr Handle => _handle;

        public Window(Arena arena, string title = "Test window", uint width = 800, uint height = 800, bool vsync = true, bool fullscreen = false)
        {
            _arena = arena ?? throw new ArgumentNullException(nameof(arena));
            _handle = Native.CimpleUI_InitWindow(arena.Handle, title, width, height, vsync, fullscreen);
            if (_handle == IntPtr.Zero)
                throw new Exception("Failed to create window");

        }

        public uint Width => Native.CimpleUI_GetWindowWidth(_handle);
        public uint Height => Native.CimpleUI_GetWindowHeight(_handle);

        public void Clear(ColorRGBA color) => Native.CimpleUI_ClearScreen(_handle, color);
        public void Present() => Native.CimpleUI_Present(_handle);

        public void Dispose()
        {
            if (_handle != IntPtr.Zero)
            {
                Native.CimpleUI_DestroyWindow(_handle);
                _handle = IntPtr.Zero;
            }
        }
    }

    public class StringMemory : IDisposable
    {
        private IntPtr _handle;
        private Arena _arena;

        public StringMemory(Arena arena, ushort maxStrings = 128)
        {
            _arena = arena ?? throw new ArgumentNullException(nameof(arena));
            _handle = Native.CimpleUI_CreateStringMemory(arena.Handle, maxStrings);
            if (_handle == IntPtr.Zero)
                throw new Exception("Failed to create string memory");
        }

        internal IntPtr Handle => _handle;
        internal Arena Arena => _arena;

        public void Dispose()
        {
            if (_handle != IntPtr.Zero)
            {
                Native.CimpleUI_DestroyStringMemory(_handle);
                _handle = IntPtr.Zero;
            }
        }
    }

    public class FontHolder : IDisposable
    {
        private IntPtr _handle;

        public FontHolder(Arena arena, byte maxFonts = 5)
        {
            if (arena == null) throw new ArgumentNullException(nameof(arena));
            _handle = Native.CimpleUI_CreateFontHolder(arena.Handle, maxFonts);
            if (_handle == IntPtr.Zero)
                throw new Exception("Failed to create font holder");
        }

        internal IntPtr Handle => _handle;

        public void LoadFont(string fileName, byte fontSize)
        {
            if (string.IsNullOrEmpty(fileName))
                throw new ArgumentException("Font file name cannot be null or empty", nameof(fileName));
            Native.CimpleUI_LoadFont(_handle, fileName, fontSize);
        }

        public void Dispose()
        {
            if (_handle != IntPtr.Zero)
            {
                Native.CimpleUI_DestroyFonts(_handle);
                _handle = IntPtr.Zero;
            }
        }
    }

    public class UIController : IDisposable
    {
        private IntPtr _handle;
        private Arena _arena;
        private StringMemory _stringMemory;
        private Window _window;
        private FontHolder _fontHolder;

        public UIController(Arena arena, Window window, StringMemory stringMemory, FontHolder fontholder, ushort totalElements = 256)
        {
            _stringMemory = stringMemory ?? throw new ArgumentNullException(nameof(stringMemory));
            _window = window ?? throw new ArgumentNullException(nameof(window));
            _arena = arena ?? throw new ArgumentNullException(nameof(arena));
            _fontHolder = fontholder ?? throw new ArgumentNullException(nameof(fontholder));
            _handle = Native.CimpleUI_CreateUIController(arena.Handle, totalElements);
            if (_handle == IntPtr.Zero)
                throw new Exception("Failed to create UI controller");
        }

        internal IntPtr Handle => _handle;
        internal Arena Arena => _arena;
        internal Window Window => _window;
        internal StringMemory StringMemory => _stringMemory;
        internal FontHolder FontHolder => _fontHolder;

        public void EventCheck()
        {
            if (Native.CimpleUI_event_check(_window.Handle, _arena.Handle, _stringMemory.Handle, _handle))
            {
                //residing function
            }
        }
        public void Update(float deltaTime) => Native.CimpleUI_UpdateUI(_handle, deltaTime);

        public void Clear(ColorRGBA clearColor) => Native.CimpleUI_ClearScreen(_window.Handle, clearColor);
        public void Render() => Native.CimpleUI_RenderUI(_window.Handle, _handle);
        public void Present() => Native.CimpleUI_Present(_window.Handle);

        //all the rendering steps in one function
        public void EasyRender(ColorRGBA clearColor = default)
        {
            if (clearColor.R == 0 && clearColor.G == 0 && clearColor.B == 0 && clearColor.A == 0)
                clearColor = ColorRGBA.Black;
            Native.CimpleUI_ClearScreen(_window.Handle, clearColor);
            Native.CimpleUI_RenderUI(_window.Handle, _handle);
            Native.CimpleUI_Present(_window.Handle);
        }

        public void Dispose()
        {
            if (_handle != IntPtr.Zero)
            {
                Native.CimpleUI_DestroyUIController(_handle);
                _handle = IntPtr.Zero;
            }
        }
    }

    public class Label
    {
        private IntPtr _handle;

        public Label(UIController uiController, int x, int y, int width, int height, string text, TabPannel? tp = null, int? tab = null,
           byte fontIndex = 0, byte fontSize = 0, ColorRGBA color = default)
        {
            if (uiController == null) throw new ArgumentNullException(nameof(uiController));
            if (color.R == 0 && color.G == 0 && color.B == 0 && color.A == 0)
                color = ColorRGBA.Blue;

            _handle = Native.CimpleUI_CreateLabel(
                uiController.Window.Handle, uiController.Arena.Handle, uiController.Handle,
                x, y, width, height, text ?? string.Empty,
                uiController.FontHolder.Handle, fontIndex, fontSize, color);

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
        private UIController _uiController;

        public TextBox(UIController uiController, float x, float y, float width, float height, TabPannel? tp = null, int? tab = null,
            byte fontIndex = 0, byte fontSize = 20, ColorRGBA color = default)
        {
            _uiController = uiController ?? throw new ArgumentNullException(nameof(uiController));

            if (color.R == 0 && color.G == 0 && color.B == 0 && color.A == 0)
                color = ColorRGBA.White;

            _handle = Native.CimpleUI_CreateTextBox(
                uiController.Arena.Handle, uiController.Handle, uiController.StringMemory.Handle, uiController.FontHolder.Handle,
                fontIndex, fontSize, color, x, y, width, height);

            if (_handle == IntPtr.Zero)
                throw new Exception("Failed to create textbox");

            if (tp != null && tab != null)
            {
                TabPannel.add_elem(_handle, tp.Handle, UI_Element.TEXTBOX_ELEM, (int)tab);
            }
        }

        internal IntPtr Handle => _handle;

        public void AppendText(string text)
        {
            if (text == null) throw new ArgumentNullException(nameof(text));
            Native.CimpleUI_TextBoxAppendText(_uiController.Arena.Handle, _uiController.StringMemory.Handle, _handle, text);
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
            Native.CimpleUI_TextBox_Clear(_handle, _uiController.StringMemory.Handle, _uiController.StringMemory.Arena.Handle);
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

        public Button(UIController uiController, int x, int y, int width, int height, string text, TabPannel? tp = null, int? tab = null,
            byte fontIndex = 0, byte fontSize = 0, ColorRGBA color = default)
        {
            if (uiController == null) throw new ArgumentNullException(nameof(uiController));

            if (color.R == 0 && color.G == 0 && color.B == 0 && color.A == 0)
                color = ColorRGBA.Green;

            _handle = Native.CimpleUI_CreateButton(
                uiController.Arena.Handle, uiController.Handle, uiController.Window.Handle, uiController.FontHolder.Handle,
                fontIndex, fontSize, x, y, width, height, text ?? string.Empty, color);

            if (_handle == IntPtr.Zero)
                throw new Exception("Failed to create button");

            // Setup native callback - don't need to worry about Pointers for data and we can capture it from the lambda event declaration
            _nativeCallback = OnNativeClick;
            Native.CimpleUI_ButtonAddClickListener(uiController.Arena.Handle, _handle, _nativeCallback, IntPtr.Zero);

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
        public TabPannel(UIController uiController, string tabNames, TabPannelPossition possition = TabPannelPossition.TABPANNEL_TOP,
            int height = 50, byte fontIndex = 0, byte fontSize = 0, ColorRGBA color = default, int elemPerTab = 16)
        {
            if (uiController == null) throw new ArgumentNullException(nameof(uiController));

            if (color.R == 0 && color.G == 0 && color.B == 0 && color.A == 0)
                color = ColorRGBA.DarkGray;

            _handle = Native.CimpleUI_CreateTabPannel(uiController.Arena.Handle, uiController.Handle, uiController.Window.Handle,
                    tabNames, possition, uiController.FontHolder.Handle, fontIndex, fontSize, height, color, elemPerTab);
        }

        internal IntPtr Handle => _handle;

        public static void add_elem(IntPtr elem, IntPtr tabPannel, UI_Element uiElementType, int tab)
        {
            Native.CimpleUI_AddElementToTabPannel(tabPannel, elem, uiElementType, tab);
        }

    }

    public static class PopupNotice
    {
        public static void Create(UIController uiController, string notice, string buttonText,
            byte fontIndex = 0, int width = 300, int height = 150, ColorRGBA color = default)
        {
            if (uiController == null) throw new ArgumentNullException(nameof(uiController));

            if (color.R == 0 && color.G == 0 && color.B == 0 && color.A == 0)
                color = ColorRGBA.White;

            Native.CimpleUI_PopupNoticeInit(
                uiController.Handle, notice ?? string.Empty, buttonText ?? string.Empty,
                uiController.FontHolder.Handle, fontIndex, width, height, color);
        }
    }
}
