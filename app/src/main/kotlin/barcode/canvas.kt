// Created by Loris Witschard on 4/21/2018.
package barcode

import org.w3c.dom.CanvasRenderingContext2D
import org.w3c.dom.HTMLCanvasElement
import org.w3c.dom.get
import kotlin.browser.document

object Canvas
{
    private const val barWidth = 20.0
    private const val barHeight = 600.0
    
    private val canvas = document.getElementsByTagName("canvas")[0] as HTMLCanvasElement
    private val scene = canvas.getContext("2d") as CanvasRenderingContext2D
    
    init
    {
        canvas.height = barHeight.toInt()
    }
    
    fun update()
    {
        val bits = Barcode.serialize()
        canvas.width = (bits.size * barWidth).toInt()
        
        for((index, bit) in bits.withIndex())
        {
            scene.fillStyle = "hsl(${index / 6 * 150}, ${100 - index % 2 * 50}%, ${75 - index % 2 * 50}%)"
            scene.fillRect(index * barWidth, 0.0, barWidth, barHeight)
            
            scene.fillStyle = if(bit) "black" else "white"
            scene.fillRect(index * barWidth, barWidth, barWidth, barHeight - 2 * barWidth)
        }
    }
    
    fun clear()
    {
        scene.clearRect(0.0, 0.0, canvas.width.toDouble(), canvas.height.toDouble())
    }
    
    fun dataURL(): String
    {
        return canvas.toDataURL()
    }
}
