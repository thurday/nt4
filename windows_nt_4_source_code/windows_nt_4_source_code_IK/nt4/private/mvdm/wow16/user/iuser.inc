GetMessage		   equ	<IGetMessage>
TranslateMessage	   equ	<ITranslateMessage>
DispatchMessage 	   equ	<IDispatchMessage>
PeekMessage		   equ	<IPeekMessage>
lstrcmp 		   equ	<Ilstrcmp>
lstrcmpi		   equ	<Ilstrcmpi>
SetSysModalWindow	   equ	<ISetSysModalWindow>
SendMessage		   equ	<ISendMessage>
PostMessage		   equ	<IPostMessage>
PostAppMessage		   equ	<IPostAppMessage>
CallWindowProc		   equ	<ICallWindowProc>
;IsChild                    equ  <IIsChild>
;IsWindowVisible            equ  <IIsWindowVisible>
;IsIconic                   equ  <IIsIconic>
CallMsgFilter		   equ	<ICallMsgFilter>
GetKeyboardState	   equ	<IGetKeyboardState>
SetKeyboardState	   equ	<ISetKeyboardState>
SetCapture		   equ	<ISetCapture>
;GetQueueStatus             equ  <IGetQueueStatus>
SetTimer		   equ	<ISetTimer>
KillTimer		   equ	<IKillTimer>
;IsWindowEnabled            equ  <IIsWindowEnabled>
GetSystemMetrics	   equ	<IGetSystemMetrics>
GetMenu 		   equ	<IGetMenu>
GetMenuState		   equ	<IGetMenuState>
DrawMenuBar		   equ	<IDrawMenuBar>
DestroyMenu		   equ	<IDestroyMenu>
CheckMenuItem		   equ	<ICheckMenuItem>
EnableMenuItem		   equ	<IEnableMenuItem>
GetSubMenu		   equ	<IGetSubMenu>
GetMenuItemID		   equ	<IGetMenuItemID>
SetActiveWindow 	   equ	<ISetActiveWindow>
BeginPaint		   equ	<IBeginPaint>
InvalidateRect		   equ	<IInvalidateRect>
ValidateRect		   equ	<IValidateRect>
InvalidateRgn		   equ	<IInvalidateRgn>
ValidateRgn		   equ	<IValidateRgn>
SetWindowText		   equ	<ISetWindowText>
GetWindowText		   equ	<IGetWindowText>
GetWindowTextLength	   equ	<IGetWindowTextLength>
;GetClientRect              equ  <IGetClientRect>
;GetWindowRect              equ  <IGetWindowRect>
GetCursorPos		   equ	<IGetCursorPos>
ClipCursor		   equ	<IClipCursor>
GetClipCursor		   equ	<IGetClipCursor>
;ClientToScreen             equ  <IClientToScreen>
;ScreenToClient             equ  <IScreenToClient>
;GetSysColor                equ  <IGetSysColor>
DrawFocusRect		   equ	<IDrawFocusRect>
FillRect		   equ	<IFillRect>
FrameRect		   equ	<IFrameRect>
InvertRect		   equ	<IInvertRect>
;SetRect                   equ  <ISetRect>
;SetRectEmpty               equ  <ISetRectEmpty>
;CopyRect                   equ  <ICopyRect>
;InflateRect                equ  <IInflateRect>
;IntersectRect              equ  <IIntersectRect>
;UnionRect                  equ  <IUnionRect>
;SubtractRect               equ  <ISubtractRect>
;OffsetRect                 equ  <IOffsetRect>
;IsRectEmpty                equ  <IIsRectEmpty>
;EqualRect                  equ  <IEqualRect>
;PtInRect                   equ  <IPtInRect>
;GetWindowWord              equ  <IGetWindowWord>
SetWindowWord		   equ	<ISetWindowWord>
;GetWindowLong              equ  <IGetWindowLong>
SetWindowLong		   equ	<ISetWindowLong>
GetClassWord		   equ	<IGetClassWord>
SetClassWord		   equ	<ISetClassWord>
GetClassLong		   equ	<IGetClassLong>
SetClassLong		   equ	<ISetClassLong>
EnumTaskWindows 	   equ	<IEnumTaskWindows>
;MapWindowPoints            equ  <IMapWindowPoints>
GetWindowTask		   equ	<IGetWindowTask>
SetWindowsHook		   equ	<ISetWindowsHook>
UnhookWindowsHook	   equ	<IUnhookWindowsHook>
DefHookProc		   equ	<IDefHookProc>
SetWindowsHookEx	   equ	<ISetWindowsHookEx>
UnhookWindowsHookEx	   equ	<IUnhookWindowsHookEx>
CallNextHookEx		   equ	<ICallNextHookEx>

DrawText		   equ	<IDrawText>
GetTabbedTextExtent	   equ	<IGetTabbedTextExtent>
TabbedTextOut		   equ	<ITabbedTextOut>
DrawIcon		   equ	<IDrawIcon>
GetWindowDC		   equ	<IGetWindowDC>
GetDC			   equ	<IGetDC>
ReleaseDC		   equ	<IReleaseDC>
GetDCEx 		   equ	<IGetDCEx>
DefWindowProc		   equ	<IDefWindowProc>
RegisterClass		   equ	<IRegisterClass>
UnregisterClass 	   equ	<IUnregisterClass>
GetClassInfo		   equ	<IGetClassInfo>
GetClassName		   equ	<IGetClassName>
GetNextQueueWindow	   equ	<IGetNextQueueWindow>
CreateWindowEx		   equ	<ICreateWindowEx>
DestroyWindow		   equ	<IDestroyWindow>
ShowWindow		   equ	<IShowWindow>
FlashWindow		   equ	<IFlashWindow>
ShowOwnedPopups 	   equ	<IShowOwnedPopups>
OpenIcon		   equ	<IOpenIcon>
CloseWindow		   equ	<ICloseWindow>
MoveWindow		   equ	<IMoveWindow>
SetWindowPos		   equ	<ISetWindowPos>
DeferWindowPos		   equ	<IDeferWindowPos>
EndDeferWindowPos	   equ	<IEndDeferWindowPos>
CreateDialogParam	   equ	<ICreateDialogParam>
CreateDialogIndirectParam  equ	<ICreateDialogIndirectParam>
DialogBoxParam		   equ	<IDialogBoxParam>
DialogBoxIndirectParam	   equ	<IDialogBoxIndirectParam>
EndDialog		   equ	<IEndDialog>
GetDlgItem		   equ	<IGetDlgItem>
GetDlgItemInt		   equ	<IGetDlgItemInt>
SetDlgItemText		   equ	<ISetDlgItemText>
GetDlgItemText		   equ	<IGetDlgItemText>
CheckDlgButton		   equ	<ICheckDlgButton>
IsDlgButtonChecked	   equ	<IIsDlgButtonChecked>
SendDlgItemMessage	   equ	<ISendDlgItemMessage>
GetNextDlgGroupItem	   equ	<IGetNextDlgGroupItem>
GetNextDlgTabItem	   equ	<IGetNextDlgTabItem>
GetDlgCtrlID		   equ	<IGetDlgCtrlID>
DefDlgProc		   equ	<IDefDlgProc>
OpenClipboard		   equ	<IOpenClipboard>
SetClipboardViewer	   equ	<ISetClipboardViewer>
ChangeClipboardChain	   equ	<IChangeClipboardChain>
GetPriorityClipboardFormat equ	<IGetPriorityClipboardFormat>
SetFocus		   equ	<ISetFocus>
EnableWindow		   equ	<IEnableWindow>
TranslateAccelerator	   equ	<ITranslateAccelerator>
LoadAccelerators	   equ	<ILoadAccelerators>
LoadMenu		   equ	<ILoadMenu>
LoadMenuIndirect	   equ	<ILoadMenuIndirect>
SetMenu 		   equ	<ISetMenu>
ChangeMenu		   equ	<IChangeMenu>
HiliteMenuItem		   equ	<IHiliteMenuItem>
GetMenuString		   equ	<IGetMenuString>
GetSystemMenu		   equ	<IGetSystemMenu>
SetSystemMenu		   equ	<ISetSystemMenu>
GetMenuItemCount	   equ	<IGetMenuItemCount>
InsertMenu		   equ	<IInsertMenu>
AppendMenu		   equ	<IAppendMenu>
ModifyMenu		   equ	<IModifyMenu>
RemoveMenu		   equ	<IRemoveMenu>
DeleteMenu		   equ	<IDeleteMenu>
SetMenuItemBitmaps	   equ	<ISetMenuItemBitmaps>
TrackPopupMenu		   equ	<ITrackPopupMenu>
GrayString		   equ	<IGrayString>
UpdateWindow		   equ	<IUpdateWindow>
EndPaint		   equ	<IEndPaint>
GetUpdateRect		   equ	<IGetUpdateRect>
GetUpdateRgn		   equ	<IGetUpdateRgn>
ExcludeUpdateRgn	   equ	<IExcludeUpdateRgn>
RedrawWindow		   equ	<IRedrawWindow>
LockWindowUpdate	   equ	<ILockWindowUpdate>
ScrollDC		   equ	<IScrollDC>
ScrollWindowEx		   equ	<IScrollWindowEx>
SetScrollPos		   equ	<ISetScrollPos>
GetScrollPos		   equ	<IGetScrollPos>
SetScrollRange		   equ	<ISetScrollRange>
GetScrollRange		   equ	<IGetScrollRange>
ShowScrollBar		   equ	<IShowScrollBar>
EnableScrollBar 	   equ	<IEnableScrollBar>
SetProp 		   equ	<ISetProp>
;GetProp                    equ  <IGetProp>
RemoveProp		   equ	<IRemoveProp>
EnumProps		   equ	<IEnumProps>
AdjustWindowRectEx	   equ	<IAdjustWindowRectEx>
MessageBox		   equ	<IMessageBox>
SetCursor		   equ	<ISetCursor>
CreateCaret		   equ	<ICreateCaret>
HideCaret		   equ	<IHideCaret>
ShowCaret		   equ	<IShowCaret>
ChildWindowFromPoint	   equ	<IChildWindowFromPoint>
SetSysColors		   equ	<ISetSysColors>
GetParent		   equ	<IGetParent>
SetParent		   equ	<ISetParent>
EnumChildWindows	   equ	<IEnumChildWindows>
FindWindow		   equ	<IFindWindow>
EnumWindows		   equ	<IEnumWindows>
GetTopWindow		   equ	<IGetTopWindow>
GetLastActivePopup	   equ	<IGetLastActivePopup>
GetWindow		   equ	<IGetWindow>
;InvalidateDCCache          equ  <IInvalidateDCCache>
LoadBitmap		   equ	<ILoadBitmap>
LoadCursor		   equ	<ILoadCursor>
CreateCursor		   equ	<ICreateCursor>
DestroyCursor		   equ	<IDestroyCursor>
LoadIcon		   equ	<ILoadIcon>
CreateIcon		   equ	<ICreateIcon>
DestroyIcon		   equ	<IDestroyIcon>
LoadString		   equ	<ILoadString>
IsDialogMessage 	   equ	<IIsDialogMessage>
MapDialogRect		   equ	<IMapDialogRect>
DlgDirList		   equ	<IDlgDirList>
DlgDirSelect		   equ	<IDlgDirSelect>
DlgDirSelectEx             equ  <IDlgDirSelectEx>
DlgDirListComboBox	   equ	<IDlgDirListComboBox>
DlgDirSelectComboBox	   equ	<IDlgDirSelectComboBox>
DlgDirSelectComboBoxEx     equ  <IDlgDirSelectComboBoxEx>
DefFrameProc		   equ	<IDefFrameProc>
DefMDIChildProc 	   equ	<IDefMDIChildProc>
TranslateMDISysAccel	   equ	<ITranslateMDISysAccel>
ArrangeIconicWindows	   equ	<IArrangeIconicWindows>
SystemParametersInfo	   equ	<ISystemParametersInfo>
WinHelp 		   equ	<IWinHelp>
OpenDriver		   equ	<IOpenDriver>
CloseDriver		   equ	<ICloseDriver>
GetDriverModuleHandle	   equ	<IGetDriverModuleHandle>
SendDriverMessage	   equ	<ISendDriverMessage>
DefDriverProc		   equ	<IDefDriverProc>
GetNextDriver		   equ	<IGetNextDriver>
GetDriverInfo		   equ	<IGetDriverInfo>
SelectPalette		   equ	<ISelectPalette>
RealizePalette		   equ	<IRealizePalette>
wsprintf		   equ	<Iwsprintf>
wvsprintf		   equ	<Iwvsprintf>