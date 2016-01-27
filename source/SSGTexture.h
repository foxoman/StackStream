/****************************************************************************
**
** Portions (C) 2015 The Qt Company Ltd.
** Portions (C) 2016 Erik Hvatum
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtQuick module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#pragma once

class SSGTexture
  : public QObject
{
    Q_OBJECT
public:
    enum WrapMode {
        Repeat,
        ClampToEdge
    };

    enum Filtering {
        None,
        Nearest,
        Linear
    };

    SSGTexture();
    virtual ~SSGTexture();

    void setOwnsTexture(bool owns) { m_owns_texture = owns; }
    bool ownsTexture() const { return m_owns_texture; }

    virtual void setTextureId(int id);
    virtual int textureId() const;
    void setTextureSize(const QSize &size) { m_texture_size = size; }
    QSize textureSize() const { return m_texture_size; }
    bool hasAlphaChannel() const { return m_has_alpha; }
    bool hasMipmaps() const { return minMipmapFiltering() != SSGTexture::None || magMipmapFiltering() != SSGTexture::None; }

    virtual QRectF normalizedTextureSubRect() const;

    virtual bool isAtlasTexture() const;

    virtual SSGTexture* removedFromAtlas() const;

    virtual void setImage(const QImage &image);
    virtual const QImage &image() { return m_image; }

    virtual void bind();
    virtual void updateBindOptions(bool force = false);

    void setMinMipmapFiltering(Filtering filter);
    SSGTexture::Filtering minMipmapFiltering() const;

    void setMagMipmapFiltering(Filtering filter);
    SSGTexture::Filtering magMipmapFiltering() const;

    void setMinFiltering(Filtering filter);
    SSGTexture::Filtering minFiltering() const;

    void setMagFiltering(Filtering filter);
    SSGTexture::Filtering magFiltering() const;

    void setHorizontalWrapMode(WrapMode hwrap);
    SSGTexture::WrapMode horizontalWrapMode() const;

    void setVerticalWrapMode(WrapMode vwrap);
    SSGTexture::WrapMode verticalWrapMode() const;

    inline QRectF convertToNormalizedSourceRect(const QRectF &rect) const {
        QSize s = textureSize();
        QRectF r = normalizedTextureSubRect();

        qreal sx = r.width() / s.width();
        qreal sy = r.height() / s.height();

        return QRectF(r.x() + rect.x() * sx,
                      r.y() + rect.y() * sy,
                      rect.width() * sx,
                      rect.height() * sy);
    }

    static SSGTexture *fromImage(const QImage &image) {
        SSGTexture *t = new SSGTexture();
        t->setImage(image);
        return t;
    }

protected:
    QImage m_image;

    uint m_wrapChanged : 1;
    uint m_filteringChanged : 1;

    uint m_horizontalWrap : 1;
    uint m_verticalWrap : 1;
    uint m_minMipmapMode : 2;
    uint m_magMipmapMode : 2;
    uint m_minFilterMode : 2;
    uint m_magFilterMode : 2;

    GLuint m_texture_id;
    QSize m_texture_size;
    QRectF m_texture_rect;

    uint m_has_alpha : 1;
    uint m_dirty_texture : 1;
    uint m_dirty_bind_options : 1;
    uint m_owns_texture : 1;
    uint m_mipmaps_generated : 1;
    uint m_retain_image: 1;
};

bool ssg_safeguard_texture(SSGTexture *);
