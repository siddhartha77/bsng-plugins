/*
 * $Workfile:: BSNG API.h                                                     $
 * $Revision:: 4                                                              $
 *
 * $Author:: Buck Rogers                                                      $
 * $Modtime:: 11.03.1998 04:20 Uhr                                            $
 *
 * $History:: BSNG API.h                                                      $
 * 
 * *****************  Version 4  *****************
 * User: Buck Rogers  Date: 11.03.1998   Time: 04:21 Uhr
 * Updated in $/BSNG/Plugins/BSNG SDK
 * added new controls like popup menus, checkboxes and radio buttons to the
 * API
 * 
 * *****************  Version 3  *****************
 * User: Buck Rogers  Date: 29.10.1997   Time: 16:01 Uhr
 * Updated in $/BSNG/Plugins/BSNG SDK
 * added errorText to display custom error messages
 * 
 * *****************  Version 2  *****************
 * User: Buck Rogers  Date: 05.10.1997   Time: 19:12 Uhr
 * Updated in $/BSNG/Plugins/BSNG SDK
 * Added name, company and number of copies items that contain the default
 * values set in the BSNG App preferences
 * 
 * *****************  Version 1  *****************
 * User: Buck Rogers  Date: 30.09.1997   Time: 18:31 Uhr
 * Created in $/BSNG/Plugins/BSNG SDK
 * Adding subproject 'BSNG' to '$/'
 *
 * $NoKeywords::                                                              $
 */


#pragma once


#if PRAGMA_ALIGN_SUPPORTED
#pragma options align=mac68k
#endif

/*
	These are the IDs of the dialog/panel items you can use.
	MAKE SURE THAT EVERY ITEM YOU ADD TO YOUR LAYOUT IS WITHIN THAT RANGE AND SENDS MESSAGE VALUES INSIDE THAT RANGE!!!
	
	Note:	The only items that need special ID and/or message ranges are:
			LGAEditFields (ID range: between kEditItem1 = 100001 and kEditItem10 = 100010)
			LPopupButtons (ID and message range: between kPopupItem1 = 200001 and kPopupItem10 = 200010)
			LGAPushButtons (ID and message range: between 300000 and 399999)
			LCheckboxes (ID and message range: between kCheckboxItem1 = 400001 and kCheckboxItem10 = 400010)
			LRadioButtons (ID and message range: between kRadioItem1 = 500001 and kRadioItem10 = 500010)
	
	Everything else you add, like LGACaptions or LGAFocusBorders should have an ID smaller than 100000 though to
	avoid possible conflicts with the BSNG App in the future.
	
	ADDITIONAL NOTE!!! (read for even more fun)
		I tested the following control elements: 
		(for all other controls you might want to test you still have to use the appropiate IDs and value messages for each control.
		That means if you want to use LGAPopups instead of LPopupButtons you still have to use the IDs/messages I mentioned
		above - kPopupItem1...kPopupItem10)
		
		LGAEditField	for TextEdit fields (NO OTHER WILL WORK!!!, sorry)
		LPopupButton	for popup menus (other similar controls like LGAPopup or LBevelButton might work
		LGAPushButtons	for "normal" buttons (other similar controls like LGATextButton or LGAIconButton in button mode might work)
		LCheckboxes		for checkboxes (other similar controls like LGACheckBox or LGAIconButton in switch mode might work)
		LRadioButton	for radio buttons (other similar controls like LGARadioButton or LGAIconButton in radio button mode might work)
		
		If you use other controls than those I tested you'll know if they work as soon as you try out your plugin. If the BSNG App quits
		or freezes when you select your plugin one or more of the controls you use don't work. If you click on the controls and
		nothing happens the control either doesn't work or you set the wrong control IDs/value messages for that control.
		
		Nevertheless, I hope you'll have lots of fun with these new additions!!!
*/

enum EditFieldItem		{kEditItem1 = 100001, kEditItem2 = 100002, kEditItem3 = 100003, kEditItem4 = 100004, kEditItem5 = 100005,
						 kEditItem6 = 100006, kEditItem7 = 100007, kEditItem8 = 100008, kEditItem9 = 100009, kEditItem10 = 100010};	

enum PopupMenuItem		{kPopupItem1 = 200001, kPopupItem2 = 200002, kPopupItem3 = 200003, kPopupItem4 = 200004, kPopupItem5 = 200005,
						 kPopupItem6 = 200006, kPopupItem7 = 200007, kPopupItem8 = 200008, kPopupItem9 = 200009, kPopupItem10 = 200010};	

enum CheckboxItem		{kCheckboxItem1 = 400001, kCheckboxItem2 = 400002, kCheckboxItem3 = 400003, kCheckboxItem4 = 400004, kCheckboxItem5 = 400005,
						 kCheckboxItem6 = 400006, kCheckboxItem7 = 400007, kCheckboxItem8 = 400008, kCheckboxItem9 = 400009, kCheckboxItem10 = 400010};	

enum RadioItem			{kRadioItem1 = 500001, kRadioItem2 = 500002, kRadioItem3 = 500003, kRadioItem4 = 500004, kRadioItem5 = 500005,
						 kRadioItem6 = 500006, kRadioItem7 = 500007, kRadioItem8 = 500008, kRadioItem9 = 500009, kRadioItem10 = 500010};	

/*
	These are constants you can use to access the itemValue array. If you want to access the value the user entered in your
	LGAEditField with the ID kEditItem2 you can use itemValue[1] or for more readability itemValue[kItemValue2]
*/

enum EditFieldValue		{kItemValue1 = 0, kItemValue2 = 1, kItemValue3 = 2, kItemValue4 = 3, kItemValue5 = 4,
						 kItemValue6 = 5, kItemValue7 = 6, kItemValue8 = 7, kItemValue9 = 8, kItemValue10 = 9};


/*
	These are constants you can use to access the selectedPopupItem array. If you want to access the menu item the user selected in your
	LPopupButton with the ID kPopupItem3 you can use selectedPopupItem[2] or for more readability selectedPopupItem[kPopupMenu3]
*/

enum PopupMenu			{kPopupMenu1 = 0, kPopupMenu2 = 1, kPopupMenu3 = 2, kPopupMenu4 = 3, kPopupMenu5 = 4,
						 kPopupMenu6 = 5, kPopupMenu7 = 6, kPopupMenu8 = 7, kPopupMenu9 = 8, kPopupMenu10 = 9};


/*
	These are constants you can use to access the checkboxSelected array. If you want to know if the user selected/checked your
	LCheckbox with the ID kCheckboxItem5 you can use checkboxSelected[4] or for more readability checkboxSelected[kCheckbox5]
*/

enum Checkbox			{kCheckbox1 = 0, kCheckbox2 = 1, kCheckbox3 = 2, kCheckbox4 = 3, kCheckbox5 = 4,
						 kCheckbox6 = 5, kCheckbox7 = 6, kCheckbox8 = 7, kCheckbox9 = 8, kCheckbox10 = 9};


/*
	These are constants you can use to access the radioSelected array. If you want to know if the user selected/checked your
	LRadioButton with the ID kRadioItem9 you can use radioSelected[8] or for more readability radioSelected[kRadioButton9]
	
	Note:	if you correctly create radio button groups in Constructor the BSNG App will take care of correct radio button
			behaviour (if you select another radio button than the currently selected one in the radio group the button
			belongs to all other radio buttons of that group will automatically be disabled). If you don't define radio button
			groups you have to simulate that behaviour manually in the 'DoItemHit' function of your plugin by setting the
			correct values in the radioSelected array whenever a radio button gets hit/pressed.
*/

enum RadioButton		{kRadioButton1 = 0, kRadioButton2 = 1, kRadioButton3 = 2, kRadioButton4 = 3, kRadioButton5 = 4,
						 kRadioButton6 = 5, kRadioButton7 = 6, kRadioButton8 = 7, kRadioButton9 = 8, kRadioButton10 = 9};


/*
	Here are the descriptions of all the possible messages your plugin can recieve:

	msgExtInit 				Will be passed when your plugin gets loaded.
							This is the time to set the 'wantsRandomButton' and 'canAddToSNList' values and to initialize your
							global variables. You should also load all additional resources (if you use any in your plug) that
							are needed for your calculations, like CRC tables etc., at this point of time.
							If your initialisation failed for some reason report that back to the BSNG App by setting 'error'
							to errExtInitFailed, if your init was successful set 'error' to errExtNoErr
	
	msgExtCalcHit			Will be passed when the user pressed the 'Calculate' button in the BSNG App.
							You should validate all necessary data for your calculations (passed to you in the 'itemValue' array)
							before you start calculating. set 'error' to errExtIncorrectValue with the corresponding
							EditFieldItem in 'errorInItem' back to the BSNG App if the validation failed.
							If the validation was succesful start your calculation and pass the result back in the appropiate entry
							in the 'itemValue' array (e.g. use itemValue[kItemValue2] if your output LGAEditField has the ID kEditItem2,
							use itemValue[kItemValue3] if your output LGAEditField has the ID kEditItem3 etc).
	
	msgExtRandomHit			You will only receive that message if you set 'wantsRandomButton' to true at Init time.
							Create a random serial number not dependent on user input and fill in all the necessary data into the
							'itemValue' array to display your calculations (see msgExtCalcHit how to do that)
	
	msgExtItemHit			You will receive that message only when you added one or more LPopupButtons, LGAPushButtons, LCheckboxes
							and/or LRadioButtons to your layout.
							The IDs and control messages have to be in the range listed above to make this work. The message
							of the control that was pressed will be passed to you in 'itemMessage'.
							You can do lots of things with this feature, like display your own about box, play a sound or whatever
							else comes to your mind. Actually this is very powerful because you get all control you need to
							'customize' your plugin, but please keep in mind not to 'bloat' your plugin by adding too much fancy stuff.
							All the values you probably changed for some reason in the 'itemValue' array or one of the other value arrays
							will be displayed to the user when the BSNG App gets control again. Have fun :-)
							
	
	msgExtCreateRandom		Your plugin will only get this message if you set 'canAddToSNList' to true at Init time.
							Use the 'outputRefNum' to write your randomly calculated serial numbers to the filestream.
							Use FSWrite and/or the helper functions myOutputPStr, myOutputSuffixedPStr, myOutputText
							and myOutputSuffixedText from "standard utils" (included with this package) to write to the filestream.
							You should calculate 'numOfListNumbers' random numbers if you can and write them to the stream...
								
	msgExtCleanup			Will be passed when your plugin gets unloaded.
							All memory allocations you made at Init time or whenever and all the resources you loaded should be
							deallocated/freed/dissposed when your plugin receives this message to avoid memory leaks.
*/ 


enum ExternalMessage	{msgExtInit = 1, msgExtCalcHit = 2, msgExtRandomHit = 3, msgExtItemHit = 4, msgExtCreateRandom = 5, msgExtCleanup = 6};

/*
	Here are the descriptions of all supported errors at the moment. If you pass another value back to the BSNG App than those
	listed here the user will be prompted with an 'the plugin reported an unknown error, please continue with caution' alert
	
	errExtInitFailed		Your plugin failed to initialize itself at Init time
	
	errExtIncorrectValue	report that error at msgExtCalcHit time if the user entered an incorrect value into one of the LGAEditField.
							Report the field containing the error back to the BSNG App by setting 'errorInItem' to the
							corresponding LGAEditField ID (kEditItem1...kEditItem10)
	
	errExtWritingToList		report that error when you got the msgExtCreateRandom message and for one or the other reason can't
							write your data to the list. If you report that error the creation of the complete list will be aborted.
	
	errExtNoErr				report this if everything works fine :-)
*/

enum ExternalError		{errExtInitFailed = -100000, errExtIncorrectValue = -100001, errExtWritingToList = -100002, errExtNoErr = 0};


typedef struct
{
	short				theMessage;				/*	set by the BSNG App, you have to react to these messages */
												/*	(see ExternalMessage above) */
	
	unsigned long		itemMessage;			/*	if you added one or more LPopupButtons, LGAPushButtons, LCheckboxes and/or */
												/*	LRadioButtons to your layout you'll find the message you configured for that */
												/*	control in here when it got pressed/selected. The control IDs and messages MUST */
												/*	be within the ranges mentioned above to make it work */
	
	Boolean				wantsRandomButton;		/*	set to true at Init time if your plugin can create random serial numbers */
	
	Boolean				canAddToSNList;			/*	set to true at Init time if your plugin can create numbers for the */
												/*	Random Serial Number List */
	
	Str63				name;					/*	name, company and numCopies are default values entered in the BSNG App preferences. */
	Str63				company;				/*	if possible copy these in values in the corresponding itemValue items at Init time */
	Str15				numCopies;				/*	so that these values can appear as default in your generator. Also make sure that */
												/*	you make a serial number using these infos when the Serial Number List gets */
												/*	created (of course only if your plugin is a name-based algorithm) */
	
	unsigned short		numOfListNumbers;		/*	how many numbers to create for the random Serial Number List */
	
	short				outputRefNum;			/*	the reference number of the file to write to when we build the */
												/*	random Serial Number List */
	
	long				error;					/*	if an error occured put the appropiate error number (see 'ExternalError' above) */
												/*	in here. You can create your own custom errors by setting this to a positive, */
												/*	non-zero value and putting your error text into the 'errorText' variable */
												
	unsigned long		errorInItem;			/*	if the error that occured is of type 'errExtIncorrectValue' put the
												/*	appropiate 'EditFieldItem' in here. Lets say the wrong value is in itemValue[3] */
												/*	which is equal to itemValue[kItemValue4] put kEditItem4 into this field */
	
	unsigned long		randSeed1;				/*	set by the BSNG App, you can use these two randSeed values to initialize the */
	unsigned long		randSeed2;				/*	Ultra Random Number Generator (source included) or any other Random Number Gen */
	
	unsigned long		appLaunchTime;			/*	set by the BSNG App, the time the BSNG App was launched in Ticks */
	DateTimeRec			appLaunchDate;			/*	set by the BSNG App, the time the BSNG App was launched in a DateTimeRec */
	
	StringPtr			itemValue[10];			/*	You can add up to 10 LGAEditFields to your layout. The IDs of your LGAEditField */
												/*	MUST be between 100001 and 100010 (kEditItem1...kEditItem10). */
	
	Str255				errorText;				/*	Whenever you signal an error in the 'error' variable you can make the BSNG app */
												/*	to display your own error text. Just put the string you want to show into this */
												/*	variable and the BSNG app will display it in an Alert. Your error text can be */
												/*	max. 255 characters long and has to be a pascal string. If you leave this string */
												/*	empty the BSNG app will display its own error alert as before. */
												/*	You can now also create your own error conditions, whenever you encounter an */
												/*	error other than those predefined set the 'error' variable to a positive, */
												/*	non-zero value and put the text you want to display into this variable */
												/*	Thanks to Mididman for suggesting this errorText variable */
	
	long				selectedPopupItem[10];	/*	You can add up to 10 LPopupButtons to your layout. The IDs and value messages of */
												/*	your LPopupButtons MUST be between 200001 and 200010 (kPopupItem1...kPopupItem10). */
												/*	The currently selected menu item of every popup is stored in this array */
	
	Boolean				checkboxSelected[10];	/*	You can add up to 10 LCheckboxes to your layout. The IDs and value messages of */
												/*	your LCheckboxes MUST be between 400001 and 400010 (kCheckboxItem1... */
												/*	kCheckboxItem10). The current status of every checkbox (unchecked = false, */
												/*	checked = true) is stored in this array */
	
	Boolean				radioSelected[10];		/*	You can add up to 10 LRadioButtons to your layout. The IDs and value messages of */
												/*	your LRadioButtons MUST be between 500001 and 500010 (kRadioItem1...kRadioItem10). */
												/*	The current status of every radio button (unselected = false, selected = true) */
												/*	is stored in this array */
	
} BSNGParamBlock, *BSNGParamBlockPtr;


/*
	IMPORTANT!!! more additional notes:
	
	You MUST create the following resouces, otherwise your plugin will not work:
	
	'cicn' ID 1000	must contain the icon displayed in the plugin list in the BSNG App
	'STR ' ID 1000	must contain the text displayed under the icon, the max. supported length of this text is 27 characters
	'PPob' ID 1000	must contain the layout displayed on the right side of the serial number generator window when your plugin
					gets selected. Use Metrowerks Constructor 2.4.4a1 or better (included in this distribution) to customize this view.
					
					YOU CAN'T USE/CONSTRUCT THE NEW FEATURES LIKE POPUPS AND CHECKBOXES CORRECTLY WITH EARLIER VERSIONS OF CONTRUCTOR!!!
*/

#if PRAGMA_ALIGN_SUPPORTED
#pragma options align=reset
#endif
