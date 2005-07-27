/* This file is part of the KDE libraries
   Copyright (C) 2002-2005 Hamish Rodda <rodda@kde.org>
   Copyright (C) 2003      Anakim Border <aborder@sources.sourceforge.net>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/

#include "katetextlayout.h"

#include <kdebug.h>

#include "katelinerange.h"

KateTextLayout::KateTextLayout(KateLineLayoutPtr line, int viewLine)
  : m_lineLayout(line)
  , m_viewLine(viewLine)
  , m_startX(m_viewLine ? -1 : 0)
{
  if (isValid())
    m_textLayout = m_lineLayout->layout()->lineAt(m_viewLine);
}

bool KateTextLayout::isDirty( ) const
{
  if (!isValid())
    return true;

  return m_lineLayout->isDirty(viewLine());
}

bool KateTextLayout::setDirty( bool dirty )
{
  if (!isValid())
    return true;

  return m_lineLayout->setDirty(viewLine(), dirty);
}

bool KateTextLayout::includesCursor(const KTextEditor::Cursor& realCursor) const
{
  return realCursor.line() == line() && realCursor.column() >= startCol() && (!wrap() || realCursor.column() < endCol());
}

int KateTextLayout::xOffset() const
{
  if (!isValid())
    return 0;

  return startX() ? m_lineLayout->shiftX() : 0;
}

void KateTextLayout::debugOutput() const
{
  kdDebug() << "KateTextLayout: " << m_lineLayout << " valid " << isValid() << " line " << m_lineLayout->line() << " (" << line() << ") cols [" << startCol() << " -> " << endCol() << "] x [" << startX() << " -> " << endX() << " off " << m_lineLayout->shiftX() << "] wrap " << wrap() << endl;
}

bool operator> (const KateTextLayout& r, const KTextEditor::Cursor& c)
{
  return r.line() > c.line() || r.endCol() > c.column();
}

bool operator>= (const KateTextLayout& r, const KTextEditor::Cursor& c)
{
  return r.line() > c.line() || r.endCol() >= c.column();
}

bool operator< (const KateTextLayout& r, const KTextEditor::Cursor& c)
{
  return r.line() < c.line() || r.startCol() < c.column();
}

bool operator<= (const KateTextLayout& r, const KTextEditor::Cursor& c)
{
  return r.line() < c.line() || r.startCol() <= c.column();
}

bool KateTextLayout::isValid( ) const
{
  return m_lineLayout && m_lineLayout->isValid() && m_viewLine >= 0 && m_viewLine < m_lineLayout->viewLineCount();
}

int KateTextLayout::line( ) const
{
  if (!isValid())
    return -1;

  return m_lineLayout->line();
}

int KateTextLayout::virtualLine( ) const
{
  if (!isValid())
    return -1;

  return m_lineLayout->virtualLine();
}

int KateTextLayout::viewLine( ) const
{
  if (!isValid())
    return 0;

  return m_viewLine;
}

const QTextLine & KateTextLayout::lineLayout( ) const
{
  return m_textLayout;
}

KateLineLayoutPtr KateTextLayout::kateLineLayout( ) const
{
  return m_lineLayout;
}

int KateTextLayout::startCol( ) const
{
  if (!isValid())
    return 0;

  return lineLayout().textStart();
}

KTextEditor::Cursor KateTextLayout::start( ) const
{
  return KTextEditor::Cursor(line(), startCol());
}

int KateTextLayout::endCol( ) const
{
  if (!isValid())
    return 0;

  return startCol() + m_textLayout.textLength();
}

KTextEditor::Cursor KateTextLayout::end( ) const
{
  return KTextEditor::Cursor(line(), endCol());
}

int KateTextLayout::length( ) const
{
  if (!isValid())
    return 0;

  return m_textLayout.textLength();
}

bool KateTextLayout::isEmpty( ) const
{
  if (!isValid())
    return true;

  return startCol() == 0 && endCol() == 0;
}

bool KateTextLayout::wrap( ) const
{
  if (!isValid())
    return false;

  return viewLine() < m_lineLayout->viewLineCount() - 1;
}

int KateTextLayout::startX( ) const
{
  if (!isValid())
    return 0;

  if (m_startX == -1)
    // viewLine is already > 0, from the constructor
    for (int i = 0; i < viewLine(); ++i)
      m_startX += m_lineLayout->layout()->lineAt(i).naturalTextWidth();

  return m_startX;
}

int KateTextLayout::endX( ) const
{
  if (!isValid())
    return 0;

  return m_startX + m_textLayout.naturalTextWidth();
}

int KateTextLayout::width( ) const
{
  if (!isValid())
    return 0;

  return m_textLayout.naturalTextWidth();
}

KateTextLayout KateTextLayout::invalid( )
{
  return KateTextLayout();
}
