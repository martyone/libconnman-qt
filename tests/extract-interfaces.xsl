<?xml version="1.0"?>
<xsl:stylesheet version="2.0"
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:tp="http://telepathy.freedesktop.org/wiki/DbusSpec#extensions-v0">
    <xsl:output method="xml" indent="yes"/>

    <xsl:template name="main">
        <node>
            <xsl:for-each select="tokenize($input-files,':')">
                <xsl:apply-templates select="document(.)//interface"/>
            </xsl:for-each>
        </node>
    </xsl:template>

    <xsl:template match="@*|node()">
        <xsl:copy>
            <xsl:apply-templates select="@*|node()"/>
        </xsl:copy>
    </xsl:template>

    <xsl:template match="@tp:*|tp:*|annotation"/>

</xsl:stylesheet>
