// ******************************************************************
// *
// *    .,-:::::    .,::      .::::::::.    .,::      .:
// *  ,;;;'````'    `;;;,  .,;;  ;;;'';;'   `;;;,  .,;;
// *  [[[             '[[,,[['   [[[__[[\.    '[[,,[['
// *  $$$              Y$$$P     $$""""Y$$     Y$$$P
// *  `88bo,__,o,    oP"``"Yo,  _88o,,od8P   oP"``"Yo,
// *    "YUMMMMMP",m"       "Mm,""YUMMMP" ,m"       "Mm,
// *
// *   Cxbx->Win32->CxbxKrnl->XG.1.0.5788.cpp
// *
// *  This file is part of the Cxbx project.
// *
// *  Cxbx and Cxbe are free software; you can redistribute them
// *  and/or modify them under the terms of the GNU General Public
// *  License as published by the Free Software Foundation; either
// *  version 2 of the license, or (at your option) any later version.
// *
// *  This program is distributed in the hope that it will be useful,
// *  but WITHOUT ANY WARRANTY; without even the implied warranty of
// *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// *  GNU General Public License for more details.
// *
// *  You should have recieved a copy of the GNU General Public License
// *  along with this program; see the file COPYING.
// *  If not, write to the Free Software Foundation, Inc.,
// *  59 Temple Place - Suite 330, Bostom, MA 02111-1307, USA.
// *
// *  (c) 2002-2003 Aaron Robinson <caustik@caustik.com>
// *
// *  All rights reserved
// *
// ******************************************************************

// ******************************************************************
// * XFONT_OpenBitmapFontFromMemory
// ******************************************************************
OOVPA_NO_XREF(XFONT_OpenBitmapFontFromMemory, 5788, 8)

        { 0x0B, 0x75 },
        { 0x1A, 0x8B },
        { 0x28, 0x8B },
        { 0x32, 0x08 },
        { 0x3F, 0x8B },
        { 0x4C, 0x8B },
        { 0x59, 0x45 },
        { 0x66, 0x0C },
OOVPA_END;

// ******************************************************************
// * XG_5788
// ******************************************************************
OOVPATable XG_5788[] = {

	REGISTER_OOVPA(XGIsSwizzledFormat, 4361, PATCH),
    // REGISTER_OOVPA(XGSwizzleRect, 3911, DISABLED), // TODO : Uncomment
	// REGISTER_OOVPA(XGUnswizzleRect, 3911, DISABLED), // TODO : Uncomment
	// REGISTER_OOVPA(XFONT_OpenBitmapFontFromMemory, 5788, PATCH),
};

// ******************************************************************
// * XG_5788_SIZE
// ******************************************************************
uint32 XG_5788_SIZE = sizeof(XG_5788);
